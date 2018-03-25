#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>
#include "Controller.h"

static Controller controller;

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
void setup() {
	Serial.begin(9600);
	controller.setup();

	Serial.println("setup done");
}

// the loop function runs over and over again forever
void loop() {
	controller.handle();
}
