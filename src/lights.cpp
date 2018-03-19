#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>

#include "MotionSpot.h"
#include "Runner.h"
#include "Teleruptor.h"

// create pins

// inpinsA
InPin inpinA[] = {
		InPin(CONTROLLINO_A0),
		InPin(CONTROLLINO_A1),
		InPin(CONTROLLINO_A2),
		InPin(CONTROLLINO_A3),
		InPin(CONTROLLINO_A4),
		InPin(CONTROLLINO_A5),
		InPin(CONTROLLINO_A6),
		InPin(CONTROLLINO_A7),
		InPin(CONTROLLINO_A8),
		InPin(CONTROLLINO_A9),
};

// inpin INT
InPin inpinInt[] = {
		InPin(CONTROLLINO_IN0),
		InPin(CONTROLLINO_IN1),
};

// outpins, relay
OutPin relay[] = {
		OutPin(CONTROLLINO_RELAY_00),
		OutPin(CONTROLLINO_RELAY_01),
		OutPin(CONTROLLINO_RELAY_02),
		OutPin(CONTROLLINO_RELAY_03),
		OutPin(CONTROLLINO_RELAY_04),
		OutPin(CONTROLLINO_RELAY_05),
		OutPin(CONTROLLINO_RELAY_06),
		OutPin(CONTROLLINO_RELAY_07),
		OutPin(CONTROLLINO_RELAY_08),
		OutPin(CONTROLLINO_RELAY_09),
};

// outpins, digital
OutPin outpinD[] = {
		OutPin(CONTROLLINO_D0),
		OutPin(CONTROLLINO_D1),
		OutPin(CONTROLLINO_D2),
		OutPin(CONTROLLINO_D3),
		OutPin(CONTROLLINO_D4),
		OutPin(CONTROLLINO_D5),
		OutPin(CONTROLLINO_D6),
		OutPin(CONTROLLINO_D7),
		OutPin(CONTROLLINO_D8),
		OutPin(CONTROLLINO_D9),
};

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
Teleruptor teleruptors[] = {
		Teleruptor( inpinA[0], relay[0]) ,
		Teleruptor( inpinA[1], relay[1]),
		Teleruptor( inpinA[2], relay[2]),
		Teleruptor( inpinA[3], relay[3]),
		Teleruptor( inpinA[4], relay[4]),
		Teleruptor( inpinA[5], relay[5]),
		Teleruptor( inpinA[6], relay[6]),
		Teleruptor( inpinA[7], relay[7]),
		Teleruptor( inpinA[8], relay[8]),
		/*Teleruptor( inpinA[9], relay[9]),*/
};

// TODO define pins
// TODO define buttons

Button spotButton0(inpinInt[0]);
Button spotButton1(inpinInt[1]);
sigslot::signal0<> button0Short;
sigslot::signal0<> button0Long;
sigslot::signal0<> button1Short;
sigslot::signal0<> button1Long;
// TODO setup modes and connect to motionspot
MotionSpot spot[] =
		{
				MotionSpot(outpinD[0],outpinD[1],outpinD[2] ),
				MotionSpot(outpinD[3],outpinD[4],outpinD[5] ),
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
