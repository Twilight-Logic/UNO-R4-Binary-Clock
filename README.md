# UNO-R4-Binary-Clock

UNO R4 port of the UNO Q Binary Clock

This project is a port of the UNO Q Binary clock by phillipe86220 to the UNO R4. The original project for the
UNO Q is discussed here:

https://forum.arduino.cc/t/uno-q-binary-clock-webui-led-matrix-bridge-demo/1431807

The UNO Q has a much more sophisticated architecture to the UNO R4. The UNO Q can operate in SBC mode as well as PC USB mode and has an STMicroelectronics STM32U585 MCU as well as a Qualcom Dragonwing QRB2210 MPU that runs an ARM version of Debian Linux speficically developped for the UNO Q. It also runs Python and AppLab. The STM32U585 MCU can be programmed using the Arduino C++ based language. The UNO Q can be programmed with AppLab or in traditional fashion using the Arduino IDE.

On the other hand, the UNO R4 WiFi is built around a Renesas RA4M1 MCU and has an on-board ESP32 to provide WiFi fuctionality. It does not run Linux or Python and so is programmed using the same way that other Arduino boards are using the Arduino IDE and the Arduino C++ based language.

Phillipe explains that:

"The goal was to build a simple end-to-end example demonstrating the dual-core architecture of UNO Q" including:

Linux → Bridge → STM32 → LED Matrix
Linux → WebUI (Binary Clock)

The project shows how to:

• use Linux system time as the single time source
• send periodic updates to the STM32 via Bridge RPC
• display a digital clock on the LED matrix
• build a WebUI binary clock using the WebUI brick

The WebUI allows starting and stopping the matrix display, and the architecture clearly separates Linux logic from STM32 rendering."

While taking a look at this excellent project, the WiFi module on my UNO Q decided to die. Since I had a UNO R4 WiFi to hand and this also has a LED matrix, while waiting for my replacement UNO Q board to arrive I hit upon the idea of porting the project to the UNO R4.

Of course, since the UNO R4 WiFi does not run Linux or Python as there is no Qualcomm MPU, everything would have to run on the Renesas RA4M1 with the ESP32 providing the WiFi function to provide the Web portal. An interesting point to note here is that the approach to Wifi on the UNO R4 WiFi is not the same as when working with a separate Arduino board and ESP32 module. Arduino has integrated the two processors onto the same board and provided a communication "bridge" between the them. This consists of a special firmware running on the ESP32 module and the Arduino WiFiS3 library. By default, the code is uploaded and stored on the Renesas RA4M1. WiFi functions are accessed via the "bridge" to the ESP32. By default, the ESP32 also controls USB communication. At the time of writing, available examples showed only a basic "Hello World" type page controlling a sigle LED. Existing AsyncWebserver and similar web API libraries were not yet ported to the UNO R4, so I was going to have to develop a web back end in C++ to take place of the Python code that phillipe86220 wrote for the UNO Q.

This port is therefore inspired by the original project made public by Phillipe 86220 and also a desire to learn how to work with the WiFi features of the UNO R4 WiFi in the abscence of a web API library. I do not make any claim of its suitability for any particular purpose and have no doubt, that in time, suitable wen API libraries will appear. Most of the original UNO Q project C++, HTML and Javascript code has been retained with some minor modification, however, the web API has been re-written in C++ for the RA4M1.

There are some other key differences:

+ the LED matrix on the UNO R4 is one column shorter than on the UNO Q so the flashing dots between the two groups of digits had to be omitted. Instead, the two groups of digits are visually separated by staggering them by one row. A second counter has been added by having a LED on the bottom row counting up to 10 seconds starting from the bottom left corner, and a LED on the top row counting tens of seconds starting from the center right and moving to the top right corner.

+ there did not appear to be available any time zone library for the UNO R4 that could accept time zone names in the format accepted and used in Phillipe's JavaScript code in the browser. Instead, examples that I could find for the UNO R4 WiFi seemed to rely on being provided with a negative or positive time offset. Since, as already mentioned, Python is not available on the UNO R4 WiFi, using a Python library was not an option either. Instead, a Javascript function running in the browser was added to obtain the timezone in the format required and then extract and pass the offset information to the matrix display using the existing apiSetTimezone() API function. This function had to be slightly modified to append the offset parameter to the timezone URI query string that is was already sending. The apiSetTimezone() function also had to be called when the page is initially loaded to ensure that where timezone has already been set, the offset is updated immediately as soon as the page loads.

The default time zone and HTTP server port, along with the usual WiFi credentials can be configured in secrets.h

As with the goal stated by Phillipe, my goal was also to learn, but in this case with reference to the UNO R4 WiFi rather than the UNO Q. I wanted to understand how the matrix display worked and how to program the board using the WiFiS3 library and access its WiFi features. In this case however it is rather simpler:

RA4M1 → Matrix display
RA4M1 → Bridge → ESP32

As with the original project, the Web display works in exactly the same way and allows the matrix display to be started and stopped and demonstrates a possible implementation of a Web GUI without a specific library.

