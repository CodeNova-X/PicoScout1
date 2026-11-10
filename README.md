<img width="100%" alt="PicoScout1-Banner" src="https://github.com/user-attachments/assets/6b28ff32-2dda-4c7b-8501-e429d877b0b8" />

<br>
<br>
<br>
<br>
<br>

# PicoScout1-Intro

<br>

<p> &emsp; For a while I wanted to create my very own remote controlled mini camera robot. This is why I decided to make PicoScout for Hack Club. 
PicoScout1 will be my first version of my design and I hope to to continue to improve the project as time goes on. I want this to be open source so 
please if you try to make it, it may not work and please tell me about any improvements you make or think I should add however I may not have time to. 
This design took me hours to create and I am still not done, I spent countless hours on learning new skills and thinking about how I should design this 
robot. I researched many different things to create this from the motors I am going to use to the skills I learned to build this PCB.This is my second 
PCB I have made and I did use some AI in the process to help teach me and catch any possible mistakes.</p>

<br>
<br>

# PicoScout1-“What is it?"

<br>

<p> &emsp; PicoScout1 is my first attempt to create a controllable camera bot that will be easy to use and customize. The special thing about PicoScout1 is its custom PCB I designed. It has a small form factor which makes it easy to fit into custom robot builds and my hope is that it can make anything into a cool robot whether it be a 3d printed chassis or other things as long as it has this custom pcb with the parts, some wheels on the 2 motors, and a body </p>


<figure align=“center">
  <img alt="PicoScoutBoard-TopView" src="https://github.com/user-attachments/assets/3abe152b-eac5-463e-b04d-a4afc94dccc6" width="500" />
  <figcaption><sub>Top View of PicoScout1 PCB</sub></figcaption>
</figure>



## BOM
| Name | Purpose | Cost per item (USD) | Quantity | Total cost (USD) | Link | Distributor |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| Molex 532610271 battery connector [Bat1] | PCB component (assembled by PCBWay) | $0.17 | 1 | $0.17 | [LCSC](https://www.lcsc.com/product-detail/C177225.html) | PCBWay / LCSC |
| 22uF 1206 capacitor (Samsung CL31A226KAHNNNE) [C1] | PCB component (assembled by PCBWay) | $3.17 | 1 | $3.17 | [LCSC](https://www.lcsc.com/product-detail/C12891.html) | PCBWay / LCSC |
| 100nF 0402 capacitor (Yageo CC0402KRX7R7BB104) [C2, C4] | PCB component (assembled by PCBWay) | $0.07 | 2 | $0.13 | [LCSC](https://www.lcsc.com/product-detail/C60474.html) | PCBWay / LCSC |
| 100uF case-B tantalum capacitor (KEMET T491B107K010AT) [C3] | PCB component (assembled by PCBWay) | $1.36 | 1 | $1.36 | [LCSC](https://www.lcsc.com/product-detail/C122271.html) | PCBWay / LCSC |
| SS34 Schottky diode [D1] | PCB component (assembled by PCBWay) | $0.04 | 1 | $0.04 | [LCSC](https://www.lcsc.com/product-detail/C52023881.html) | PCBWay / LCSC |
| 22uH inductor (SRG0630-220-M) for boost converter [L1] | PCB component (assembled by PCBWay) | $0.27 | 1 | $0.27 | [LCSC](https://www.lcsc.com/product-detail/C53462993.html) | PCBWay / LCSC |
| White through-hole LED (204-15UTC/S400-X9) [LED1, LED2] | PCB component (assembled by PCBWay) | $0.40 | 2 | $0.81 | [LCSC](https://www.lcsc.com/product-detail/C2892756.html) | PCBWay / LCSC |
| MMBT3904 NPN transistor [Q1] | PCB component (assembled by PCBWay) | $0.47 | 1 | $0.47 | [LCSC](https://www.lcsc.com/product-detail/C55258612.html) | PCBWay / LCSC |
| 90.9k 0603 resistor [R1] | PCB component (assembled by PCBWay) | $0.09 | 1 | $0.09 | [LCSC](https://www.lcsc.com/product-detail/C269515.html) | PCBWay / LCSC |
| 10k 0603 resistor [R2] | PCB component (assembled by PCBWay) | $0.06 | 1 | $0.06 | [LCSC](https://www.lcsc.com/product-detail/C25804.html) | PCBWay / LCSC |
| 100k 0402 resistor [R3, R4] | PCB component (assembled by PCBWay) | $0.02 | 2 | $0.04 | [LCSC](https://www.lcsc.com/product-detail/C102750.html) | PCBWay / LCSC |
| 330 ohm 0603 resistor [R5, R6] | PCB component (assembled by PCBWay) | $0.06 | 2 | $0.12 | [LCSC](https://www.lcsc.com/product-detail/C105881.html) | PCBWay / LCSC |
| SPDT slide switch (MST-12D18G4) [SW1] | PCB component (assembled by PCBWay) | $0.51 | 1 | $0.51 | [LCSC](https://www.lcsc.com/product-detail/C49023767.html) | PCBWay / LCSC |
| MT3608 boost converter [U2] | PCB component (assembled by PCBWay) | $0.09 | 1 | $0.09 | [LCSC](https://www.lcsc.com/product-detail/C84817.html) | PCBWay / LCSC |
| DRV8835 dual motor driver [U3] | PCB component (assembled by PCBWay) | $2.27 | 1 | $2.27 | [LCSC](https://www.lcsc.com/product-detail/C129328.html) | PCBWay / LCSC |
| Seeed Studio XIAO ESP32-S3 Sense [U1] (ALREADY OWNED) | Main microcontroller with camera; already owned, not in order, value shown for reference | $21.73 | 1 | $21.73 | [Seeed Studio](https://www.seeedstudio.com/XIAO-ESP32S3-Sense-p-5639.html) | Seeed Studio |
| Custom 2-layer PCB 92x31mm 1.6mm HASL (5 pcs) | Main PCB | $5.00 | 1 | $5.00 | [PCBWay](https://www.pcbway.com) | PCBWay |
| PCBWay assembly fee | SMT assembly of PCB components | $29.00 | 1 | $29.00 | [PCBWay](https://www.pcbway.com) | PCBWay |
| PCBWay shipping | Shipping for PCB + assembly order | $33.62 | 1 | $33.62 | [PCBWay](https://www.pcbway.com) | PCBWay |
| PCBWay order discount (Winter Sale free shipping) | Discount | -$29.69 | 1 | -$29.69 | [PCBWay](https://www.pcbway.com) | PCBWay |
| PCBWay bank handling fee | Payment fee | $3.66 | 1 | $3.66 | [PCBWay](https://www.pcbway.com) | PCBWay |
| PCBWay duty & tax | Import duty and tax | $20.22 | 1 | $20.22 | [PCBWay](https://www.pcbway.com) | PCBWay |
| PCBWay DTP service fee | Service fee | $7.00 | 1 | $7.00 | [PCBWay](https://www.pcbway.com) | PCBWay |
| N20 motors (pack of 4; 2 needed) | Drive motors | $12.99 | 1 | $12.99 | [Amazon](https://www.amazon.com/dp/B0DK7635XH) | Amazon |
| Amazon shipping (N20 motors) | Free shipping | $0.00 | 1 | $0.00 | [Amazon](https://www.amazon.com/dp/B0DK7635XH) | Amazon |
| Estimated sales tax (N20 motors) | Estimated 8.9% Atlanta, GA rate on $12.99 | $1.16 | 1 | $1.16 | [Amazon](https://www.amazon.com/dp/B0DK7635XH) | Amazon |
| **TOTAL (excluding owned XIAO)** | | | | **$92.57** | | |
| **TOTAL (including owned XIAO)** | | | | **$114.30** | | |





