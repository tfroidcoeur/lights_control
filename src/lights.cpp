#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>

#include "BlinkingLed.h"
#include "Button.h"
#include "InPin.h"
#include "MotionSpot.h"
#include "Teleruptor.h"

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
Teleruptor teleruptors[] = { /* Teleruptor(CONTROLLINO_A0, CONTROLLINO_RELAY_00) ,*/
		Teleruptor(CONTROLLINO_A1, CONTROLLINO_RELAY_01), Teleruptor(
		CONTROLLINO_A2, CONTROLLINO_RELAY_02), Teleruptor(
		CONTROLLINO_A3, CONTROLLINO_RELAY_03), Teleruptor(
		CONTROLLINO_A4, CONTROLLINO_RELAY_04), Teleruptor(
		CONTROLLINO_A5, CONTROLLINO_RELAY_05), Teleruptor(CONTROLLINO_A6,
				CONTROLLINO_RELAY_06), Teleruptor(CONTROLLINO_A7,
				CONTROLLINO_RELAY_07), Teleruptor(CONTROLLINO_A8,
				CONTROLLINO_RELAY_08), /*Teleruptor(CONTROLLINO_A9,
				CONTROLLINO_RELAY_09),*/ };

BlinkPattern pat1[] = { 5, (BlinkElement[] ) { { 5000, 1 }, { 5000, 0 }, { 0, -1 } } };

BlinkPattern pat2[] = { 3, (BlinkElement[] ) { { 100, 1 }, { 200, 0 }, { 0, -1 } } };

unsigned long modes[] = { 20, 1000, 0 };

MotionSpot spot = MotionSpot(CONTROLLINO_A9,CONTROLLINO_D1, CONTROLLINO_D2, CONTROLLINO_D3);

void setup() {
	Serial.begin(9600);

	// configure input pins
	for (int i = 0; i < sizeof(teleruptors) / sizeof(class Teleruptor); i++) {
		teleruptors[i].setup();
	}

	spot.setup();
	Serial.println("setup done");
}

// the loop function runs over and over again forever
void loop() {
	for (int i = 0; i < sizeof(teleruptors) / sizeof(struct Teleruptor); i++) {
		teleruptors[i].handle();
	}

	spot.handle();
}
