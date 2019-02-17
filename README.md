# lights_control
Home lighting controller for Controllino mega

## setup
Clone the repo, `git submodule init` and `git submodule update`.

Install the `gcc-avr` compiler. On ubuntu, for example, use `sudo apt-get
install gcc-avr` to install the AVR cross compiler tools.
Install `avrdude`, `sudo apt-get install avrdude` on ubuntu.

## building
Once the setup is done, simply calling `make` in the root will build a
`lights.hex` file that can be flashed onto a Controllino maxi.

## flashing
make flash will flash using the first connected usb `ACM` device. It will assume it is a Controllino Maxi. You need to adapt the makefile in case you want to use a different USB port or target a different Controllino.

## libraries
this application makes use of following libraries:
* ArduinoSTL https://github.com/mike-matera/ArduinoSTL.git
* Ethernet Library for Arduino https://github.com/arduino-libraries/Ethernet
* Arduino AVR core https://github.com/arduino/ArduinoCore-avr
* Controllino library https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library
