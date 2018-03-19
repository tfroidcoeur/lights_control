#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>

#include "MotionSpot.h"
#include "Runner.h"
#include "Teleruptor.h"

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
Teleruptor teleruptors[] = {
		Teleruptor( CONTROLLINO_A0, CONTROLLINO_RELAY_00) ,
		Teleruptor( CONTROLLINO_A1, CONTROLLINO_RELAY_01),
		Teleruptor( CONTROLLINO_A2, CONTROLLINO_RELAY_02),
		Teleruptor( CONTROLLINO_A3, CONTROLLINO_RELAY_03),
		Teleruptor( CONTROLLINO_A4, CONTROLLINO_RELAY_04),
		Teleruptor( CONTROLLINO_A5, CONTROLLINO_RELAY_05),
		Teleruptor( CONTROLLINO_A6, CONTROLLINO_RELAY_06),
		Teleruptor( CONTROLLINO_A7, CONTROLLINO_RELAY_07),
		Teleruptor( CONTROLLINO_A8, CONTROLLINO_RELAY_08),
		/*Teleruptor( CONTROLLINO_A9, CONTROLLINO_RELAY_09),*/
};

// TODO define pins
// TODO define buttons

InPin in0(CONTROLLINO_IN0);
InPin in1(CONTROLLINO_IN1);
Button spotButton0(in0);
Button spotButton1(in1);
sigslot::signal0<> button0Short;
sigslot::signal0<> button0Long;
sigslot::signal0<> button1Short;
sigslot::signal0<> button1Long;
// TODO setup modes and connect to motionspot
MotionSpot spot[] =
		{
				MotionSpot(CONTROLLINO_D0, CONTROLLINO_D1, CONTROLLINO_D2),
				MotionSpot(CONTROLLINO_D3, CONTROLLINO_D4, CONTROLLINO_D5),
		};
Runner r;

void connectMotionSpot(MotionSpot & spot, Button & but, sigslot::signal0<> & butshort, sigslot::signal0<> & butlong) {
	but.addMode(ButtonMode(20, "short", &butshort));
	but.addMode(ButtonMode(2000, "long", &butlong));
	butshort.connect(&spot,&MotionSpot::shortpressed);
	butlong.connect(&spot,&MotionSpot::longpressed);
}

void setupMotionSpots() {
	connectMotionSpot(spot[0],spotButton0, button0Short, button0Long);
	connectMotionSpot(spot[1],spotButton1, button1Short, button1Long);

	for (unsigned long i = 0; i < sizeof(spot) / sizeof(class MotionSpot); i++) {
		r.addActor(&spot[i]);
	}
}

void setup() {
	Serial.begin(9600);

	for (unsigned int i = 0; i < sizeof(teleruptors) / sizeof(struct Teleruptor); i++) {
		r.addActor(&teleruptors[i]);
	}

	// TODO add pins and buttons
	setupMotionSpots();

	r.setup();

	Serial.println("setup done");
}

// the loop function runs over and over again forever
void loop() {
	r.handle();
}
