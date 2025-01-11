# lights_control
Home lighting controller for Controllino mega

## setup
Clone the repo, `git submodule init` and `git submodule update`.

Install the `gcc-avr` compiler. On ubuntu, for example, use `sudo apt-get
install gcc-avr` to install the AVR cross compiler tools.
Install `avrdude`, `sudo apt-get install avrdude` on ubuntu.

Or use the `setup.sh` script to build a docker and clone the submodules.

## building
Once the setup is done, simply calling `make` in the root will build a
`lights1.hex` and 'lights2.hex' (for controllino 1 and 2 respectively) file that
can be flashed onto a Controllino maxi.

Or use the `build.sh` script to run the build in the docker.

### variants
I have two different controllinos with two different configs so the Makefile
will build two variants:

* `lights1.hex` for the original, main controllino
* `lights2.hex` for the additional controllino (upstairs)


## flashing
make flash will flash using the first connected usb `ACM` device. It will assume
it is a Controllino Maxi. You need to adapt the makefile in case you want to use
a different USB port or target a different Controllino.

### raspberry pi
flashing can be done using a raspberry pi and installing avrdude.

```sh
# use lights1.hex or lights2.hex accordingly
avrdude -v -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:lights.hex:i
```


## libraries
this application makes use of following libraries:
* ArduinoSTL [https://github.com/mike-matera/ArduinoSTL.git]
* Ethernet Library for Arduino [https://github.com/arduino-libraries/Ethernet]
* Arduino AVR core [https://github.com/arduino/ArduinoCore-avr]
* Controllino library [https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library]
* Arduino NTP Client [https://github.com/arduino-libraries/NTPClient]
* Sigslot by Sarah Thompson [http://sigslot.sourceforge.net/]
