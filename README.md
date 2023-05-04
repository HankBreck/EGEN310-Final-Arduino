# Rover Controller
This repository contains the code used in the Arduino and ESP-32 modules of the EGEN-310 rover. The `rover`
directory holds the Arduino C++ code that controls the movement and cutting functionality of the rover.
The `esp32` directory holds the Arduino C++ code that facilitates communication between the controller and the Arduino.

## Dependencies
`rover` depends on the [Adafruit Motor Shield V1](https://github.com/adafruit/Adafruit-Motor-Shield-library) library. 
This library is used to control the speed and direction of each motor through the L293D Motor Shield Driver interface.
This is used to control all four DC motors and the single servo motor on the rover. 

`esp32` depends on the [ESPAsyncWebServer](https://github.com/dvarrel/ESPAsyncWebSrv) library, which uses the 
[AsyncTCP](https://github.com/dvarrel/AsyncTCP) library. 

## Building the Rover
Prior to compiling the code, the WiFi credentials must be updated. It is currently configured for a personal hotspot,
but you should update it to the credentials of the WiFi network you will connect to.

This code can be found here: [`esp32/esp32.ino:14-15`](esp32/esp32.ino#L14-L15)

Once the code is updated, you can upload each file to the Arduino and ESP-32 Development Board. Once this is complete,
you will need to wait for the ESP-32 to connect to the WiFi before the controller and rover are linked. 

> Note that the serial communication pins must be unplugged from the ESP-32 dev board when you upload the code to the Arduino.
> The serial communication channels will be rerouted to the ESP-32, which will result in an error during the upload process.

> Note that the boot button on the ESP-32 dev board must be pressed when uploading the code to it.

