/*
  PicoScout1 test sketch - XIAO ESP32-S3 Sense
  Phone joins the robot's WiFi, opens http://192.168.4.1 and gets
  live camera video + a touch joystick to drive.

  Arduino IDE setup:
    Board:  XIAO_ESP32S3   (esp32 board package 3.x)
    PSRAM:  OPI PSRAM  (Tools > PSRAM)
  Pins (read from your schematic):
    D0 MA Phase | D1 MA PWM | D3 MB Phase | D4 MB PWM | D6 LED
  DRV8835 must be in PHASE/ENABLE mode (MODE pin HIGH).
*/
#include "esp_camera.h"
#include "esp_http_server.h"
#include <WiFi.h>

// ---------- WiFi (robot makes its own network) ----------
const char* AP_SSID = "PicoScout1";
const char* AP_PASS = "picoscout";   // min 8 chars

// ---------- Pins (XIAO D-pin names) ----------
#define MA_PHASE D0
#define MA_PWM   D1
#define MB_PHASE D3
#define MB_PWM   D4
#define LED_PIN  D6

// Flip these if a motor spins the wrong way
const bool INVERT_A = false;
const bool INVERT_B = false;
// Swap if left/right are reversed
const bool SWAP_AB  = false;

// ---------- Camera pins (XIAO ESP32S3 Sense) ----------
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  10
#define SIOD_GPIO_NUM  40
#define SIOC_GPIO_NUM  39
#define Y9_GPIO_NUM    48
#define Y8_GPIO_NUM    11
#define Y7_GPIO_NUM    12
#define Y6_GPIO_NUM    14
#define Y5_GPIO_NUM    16
#define Y4_GPIO_NUM    18
#define Y3_GPIO_NUM    17
#define Y2_GPIO_NUM    15
#define VSYNC_GPIO_NUM 38
#define HREF_GPIO_NUM  47
#define PCLK_GPIO_NUM  13

httpd_handle_t ctrlHttpd = NULL;
httpd_handle_t streamHttpd = NULL;
unsigned long lastCmd = 0;

// ---------- Web page ----------
const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head>
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>PicoScout1</title>
<style>
body{margin:0;background:#111;color:#eee;font-family:sans-serif;text-align:center;touch-action:none;user-select:none}
img{width:100%;max-width:640px;background:#000}
#joy{width:220px;height:220px;border-radius:50%;background:#333;border:2px solid #666;margin:12px auto;position:relative}
#knob{width:80px;height:80px;border-radius:50%;background:#e33;position:absolute;left:70px;top:70px}
button{font-size:18px;padding:10px 20px;margin:4px}
</style></head><body>
<img id="cam">
<div id="joy"><div id="knob"></div></div>
<button onclick="fetch('/led')">Toggle LED</button>
<script>
document.getElementById('cam').src='http://'+location.hostname+':81/stream';
const joy=document.getElementById('joy'),knob=document.getElementById('knob');
let x=0,y=0,active=false;
function move(e){
  const r=joy.getBoundingClientRect(),R=r.width/2;
  let dx=e.clientX-(r.left+R),dy=e.clientY-(r.top+R);
  const d=Math.hypot(dx,dy);if(d>R){dx*=R/d;dy*=R/d;}
  knob.style.left=(R-40+dx)+'px';knob.style.top=(R-40+dy)+'px';
  x=dx/R;y=-dy/R;
}
function stop(){active=false;x=0;y=0;knob.style.left='70px';knob.style.top='70px';send();}
joy.addEventListener('pointerdown',e=>{active=true;joy.setPointerCapture(e.pointerId);move(e);});
joy.addEventListener('pointermove',e=>{if(active)move(e);});
joy.addEventListener('pointerup',stop);joy.addEventListener('pointercancel',stop);
function send(){fetch('/ctl?x='+x.toFixed(2)+'&y='+y.toFixed(2)).catch(()=>{});}
setInterval(()=>{if(active)send();},80);
</script></body></html>
)rawliteral";

// ---------- Motors ----------
void setMotor(int phasePin, int pwmPin, int speed, bool invert) {
  speed = constrain(speed, -255, 255);
  bool fwd = speed >= 0;
  if (invert) fwd = !fwd;
  digitalWrite(phasePin, fwd ? HIGH : LOW);
  ledcWrite(pwmPin, abs(speed));
}

void drive(float x, float y) {
  float l = constrain(y + x, -1.0f, 1.0f);
  float r = constrain(y - x, -1.0f, 1.0f);
  int a = (int)(l * 255), b = (int)(r * 255);
  if (SWAP_AB) { int t = a; a = b; b = t; }
  setMotor(MA_PHASE, MA_PWM, a, INVERT_A);
  setMotor(MB_PHASE, MB_PWM, b, INVERT_B);
}

// ---------- HTTP handlers ----------
static esp_err_t indexHandler(httpd_req_t* req) {
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, INDEX_HTML, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t ctlHandler(httpd_req_t* req) {
  char query[64], val[16];
  float x = 0, y = 0;
  if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
    if (httpd_query_key_value(query, "x", val, sizeof(val)) == ESP_OK) x = atof(val);
    if (httpd_query_key_value(query, "y", val, sizeof(val)) == ESP_OK) y = atof(val);
  }
  drive(x, y);
  lastCmd = millis();
  return httpd_resp_send(req, "ok", 2);
}

static esp_err_t ledHandler(httpd_req_t* req) {
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  return httpd_resp_send(req, "ok", 2);
}

static esp_err_t streamHandler(httpd_req_t* req) {
  httpd_resp_set_type(req, "multipart/x-mixed-replace;boundary=frame");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  char hdr[64];
  while (true) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) return ESP_FAIL;
    esp_err_t res = httpd_resp_send_chunk(req, "--frame\r\nContent-Type: image/jpeg\r\n", 36);
    if (res == ESP_OK) {
      int n = snprintf(hdr, sizeof(hdr), "Content-Length: %u\r\n\r\n", fb->len);
      res = httpd_resp_send_chunk(req, hdr, n);
    }
    if (res == ESP_OK) res = httpd_resp_send_chunk(req, (const char*)fb->buf, fb->len);
    if (res == ESP_OK) res = httpd_resp_send_chunk(req, "\r\n", 2);
    esp_camera_fb_return(fb);
    if (res != ESP_OK) break;   // client disconnected
  }
  return ESP_OK;
}

void startServers() {
  httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
  cfg.server_port = 80;
  httpd_uri_t idx = {"/", HTTP_GET, indexHandler, NULL};
  httpd_uri_t ctl = {"/ctl", HTTP_GET, ctlHandler, NULL};
  httpd_uri_t led = {"/led", HTTP_GET, ledHandler, NULL};
  if (httpd_start(&ctrlHttpd, &cfg) == ESP_OK) {
    httpd_register_uri_handler(ctrlHttpd, &idx);
    httpd_register_uri_handler(ctrlHttpd, &ctl);
    httpd_register_uri_handler(ctrlHttpd, &led);
  }
  cfg.server_port = 81;
  cfg.ctrl_port += 1;
  httpd_uri_t str = {"/stream", HTTP_GET, streamHandler, NULL};
  if (httpd_start(&streamHttpd, &cfg) == ESP_OK) {
    httpd_register_uri_handler(streamHttpd, &str);
  }
}

bool initCamera() {
  camera_config_t c = {};
  c.ledc_channel = LEDC_CHANNEL_0;
  c.ledc_timer = LEDC_TIMER_0;
  c.pin_d0 = Y2_GPIO_NUM; c.pin_d1 = Y3_GPIO_NUM; c.pin_d2 = Y4_GPIO_NUM; c.pin_d3 = Y5_GPIO_NUM;
  c.pin_d4 = Y6_GPIO_NUM; c.pin_d5 = Y7_GPIO_NUM; c.pin_d6 = Y8_GPIO_NUM; c.pin_d7 = Y9_GPIO_NUM;
  c.pin_xclk = XCLK_GPIO_NUM; c.pin_pclk = PCLK_GPIO_NUM;
  c.pin_vsync = VSYNC_GPIO_NUM; c.pin_href = HREF_GPIO_NUM;
  c.pin_sccb_sda = SIOD_GPIO_NUM; c.pin_sccb_scl = SIOC_GPIO_NUM;
  c.pin_pwdn = PWDN_GPIO_NUM; c.pin_reset = RESET_GPIO_NUM;
  c.xclk_freq_hz = 20000000;
  c.pixel_format = PIXFORMAT_JPEG;
  c.frame_size = FRAMESIZE_QVGA;   // 320x240 = fast; try FRAMESIZE_VGA for sharper
  c.jpeg_quality = 12;
  c.fb_count = 2;
  c.fb_location = CAMERA_FB_IN_PSRAM;
  c.grab_mode = CAMERA_GRAB_LATEST;
  return esp_camera_init(&c) == ESP_OK;
}

void setup() {
  Serial.begin(115200);

  pinMode(MA_PHASE, OUTPUT);
  pinMode(MB_PHASE, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  ledcAttach(MA_PWM, 20000, 8);   // core 3.x API
  ledcAttach(MB_PWM, 20000, 8);
  drive(0, 0);

  if (!initCamera()) Serial.println("Camera init FAILED (check PSRAM setting)");

  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.print("Join WiFi '"); Serial.print(AP_SSID);
  Serial.print("' then open http://"); Serial.println(WiFi.softAPIP());

  startServers();
}

void loop() {
  // Failsafe: stop if no joystick command for 500 ms
  if (millis() - lastCmd > 500) drive(0, 0);
  delay(20);
}
