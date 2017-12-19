/*
 * MotionSpot.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "MotionSpot.h"

unsigned long MotionSpot::modes[] = { 20, 2000, 0 };

MotionSpot::~MotionSpot() {
	// TODO Auto-generated destructor stub
}

MotionSpot::MotionSpot(int inid, int ctrlid, int forceid, int indicatorid):
			in(* (new InPin(inid))),
		ctrl(* (new OutPin(ctrlid))),
		force(* (new OutPin(forceid))),
		indicator(* (new OutPin(indicatorid))),
		button(*(new Button(in, &modes[0]))) {
	button.setListener(this);
}

void MotionSpot::handle()
{
	// button and input pin handling will notify on state change
	this->button.handle();
}

void MotionSpot::setup() {
	this->button.setup();
	this->ctrl.setup();
	this->force.setup();
}

#ifdef FOO

enum MotionSpotState {
    MS_Off,
    MS_ForcedOn,
    MS_Auto,
};

struct MotionSpot {
	InPin in;
	OutPin force;
	OutPin ctrl;
    int startTime;
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
#endif


