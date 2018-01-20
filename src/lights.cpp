#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>

#include "BlinkingLed.h"
#include "Button.h"
#include "InPin.h"
#include "MotionSpot.h"
#include "Teleruptor.h"

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
Teleruptor teleruptors[] = {  Teleruptor(CONTROLLINO_A0, CONTROLLINO_RELAY_00) ,
		Teleruptor(CONTROLLINO_A1, CONTROLLINO_RELAY_01),
		Teleruptor( CONTROLLINO_A2, CONTROLLINO_RELAY_02),
		Teleruptor( CONTROLLINO_A3, CONTROLLINO_RELAY_03),
		Teleruptor( CONTROLLINO_A4, CONTROLLINO_RELAY_04),
		Teleruptor( CONTROLLINO_A5, CONTROLLINO_RELAY_05),
		Teleruptor( CONTROLLINO_A6, CONTROLLINO_RELAY_06),
		Teleruptor( CONTROLLINO_A7, CONTROLLINO_RELAY_07),
		Teleruptor( CONTROLLINO_A8, CONTROLLINO_RELAY_08),
		Teleruptor( CONTROLLINO_A9, CONTROLLINO_RELAY_09),
};

MotionSpot spot[] =
		{
				MotionSpot(CONTROLLINO_IN0,CONTROLLINO_D0, CONTROLLINO_D1, CONTROLLINO_D2),
				MotionSpot(CONTROLLINO_IN1,CONTROLLINO_D3, CONTROLLINO_D4, CONTROLLINO_D6),
		};

void setup() {
	Serial.begin(9600);

	// configure input pins
	for (unsigned long i = 0; i < sizeof(teleruptors) / sizeof(class Teleruptor); i++) {
		teleruptors[i].setup();
	}

	for (unsigned long i = 0; i < sizeof(spot) / sizeof(class MotionSpot); i++) {
		spot[i].setup();
	}
	Serial.println("setup done");
}

// the loop function runs over and over again forever
void loop() {
	for (unsigned int i = 0; i < sizeof(teleruptors) / sizeof(struct Teleruptor); i++) {
		teleruptors[i].handle();
	}

	for (unsigned long i = 0; i < sizeof(spot) / sizeof(class MotionSpot); i++) {
		spot[i].handle();
	}
}
