#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include "inpin.h"

/*
  CONTROLLINO - Port Manipulation for MAXI and MEGA, Version 01.00
  The sketch is relevant only for CONTROLLINO variants MAXI and MEGA!

  Turns on and off four digital outputs of CONTROLLINO, which also blinks with CONTROLLINO´s status LEDs for the outputs.
  It explains how to control the microcontroller´s pins directly via port registers.

  This know how may be useful if you need to control the lines in a more efficient way, but it is essential if you
  need to control "non-Arduino" pins, like Digital Outputs 20,21,22 and 23 at CONTROLLINO MEGA.

  Please, see https://www.arduino.cc/en/Reference/PortManipulation for some theoretical background.

  List of non-Arduino pins used in CONTROLLINO boards:
  ----------------------------------------------------
   MINI - none (= all used pins have their Arduino and CONTROLLINO "nickname")
   MAXI - RS485 serial bus /RE (output)
        - RS485 serial bus DE (output)
        - Ethernet /SS (output) - Ethernet library handles this pin for you
        - Ethernet /INT (input)
        - Overload /OVL (input)
        - Real Time Clock RTC /SS (output)
        - Real Time Clock RTC /INT (input)
   MEGA - the same like for MAXI plus
        - Digital Outputs 20,21,22 and 23 (output)

  IMPORTANT INFORMATION!
  Please, select proper target board in Tools->Board->Controllino MAXI/MEGA before Upload to your CONTROLLINO.
  (Please, refer to https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library if you do not see the CONTROLLINOs in the Arduino IDE menu Tools->Board.)

  This example sketch does not require any additional parts, equipment, wires or power supply. Just your CONTROLLINO, USB cable (type A-B) and a PC.

  Created 13 Dec 2016
  by Lukas

  https://controllino.biz/

  (Check https://github.com/CONTROLLINO-PLC/CONTROLLINO_Library for the latest CONTROLLINO related software stuff.)
*/


struct OutPin {
	int id;
};

struct Teleruptor {
	struct InPin in;
	struct OutPin out;
};


enum MotionSpotState {
    MS_Off,
    MS_ForcedOn,
    MS_Auto,
};

struct MotionSpot {
	struct InPin in;
	struct OutPin force;
	struct OutPin ctrl;
    int startTime;
};

static void initOutputpin(struct OutPin * pin) {
	pinMode(pin->id, OUTPUT);
}

static void teleruptorInpinHandler(struct InPin * pin, void * data) {
	struct Teleruptor * t = (struct Teleruptor *) data;

	if (pin->getInPinValue() == HIGH) {
		// button pressed, toggle output
		digitalWrite(t->out.id, !digitalRead(t->out.id));
	}

}

static void initTeleruptor(struct Teleruptor * t) {
	t->in.setHandler(teleruptorInpinHandler, t);
	initOutputpin(&t->out);
}

static void handleTeleruptor(struct Teleruptor * t) {
	// input pin handling will call callbacks on state change
	t->in.handle();
}

struct Teleruptor teleruptors[] = {
  { .in = new InPin(CONTROLLINO_A1),
    .out = { .id = CONTROLLINO_RELAY_00 } },
  { .in = { .id = CONTROLLINO_A1 },
    .out = { .id = CONTROLLINO_RELAY_01 } },
  { .in = { .id = CONTROLLINO_A2 },
    .out = { .id = CONTROLLINO_RELAY_02 } },
  { .in = { .id = CONTROLLINO_A3 },
    .out = { .id = CONTROLLINO_RELAY_03 } },
  { .in = { .id = CONTROLLINO_A4 },
    .out = { .id = CONTROLLINO_RELAY_04 } },
  { .in = { .id = CONTROLLINO_A5 },
    .out = { .id = CONTROLLINO_RELAY_05 } },
  { .in = { .id = CONTROLLINO_A6 },
    .out = { .id = CONTROLLINO_RELAY_06 } },
  { .in = { .id = CONTROLLINO_A7 },
    .out = { .id = CONTROLLINO_RELAY_07 } },
  { .in = { .id = CONTROLLINO_A8 },
    .out = { .id = CONTROLLINO_RELAY_08 } },
  { .in = { .id = CONTROLLINO_A9 },
    .out = { .id = CONTROLLINO_RELAY_09 } },
};

static void motionSpotInHandler(struct InPin * pin, void * data) {
	struct MotionSpot * m = (struct MotionSpot *) data;

	if (pin->getInPinValue() == HIGH) {
		// button pressed, wait if long press or short
        m->startTime = millis();
	} else if (m->startTime) {
        // button released but not expired yet, short press
		digitalWrite(m->ctrl.id, !digitalRead(m->ctrl.id));
    }
}

static void initMotionSpot(struct MotionSpot * m) {
	m->in.setHandler( motionSpotInHandler, m);
	initOutputpin(&m->ctrl);
	initOutputpin(&m->force);
}

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
void setup() {
	Serial.begin(9600);

	// configure input pins
	for (int i = 0; i < sizeof(teleruptors) / sizeof(struct Teleruptor); i++) {
		initTeleruptor(&teleruptors[i]);
	}

}

// the loop function runs over and over again forever
void loop() {
	for (int i = 0; i < sizeof(teleruptors) / sizeof(struct Teleruptor); i++) {
		handleTeleruptor(&teleruptors[i]);
	}
}
