/*
 * MotionSpot.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "MotionSpot.h"

MotionSpot::~MotionSpot() {
	// TODO Auto-generated destructor stub
}

MotionSpot::MotionSpot(int inid, int ctrlid, int forceid):
			in(inid),ctrl(ctrlid),force(forceid) {
	// TODO Auto-generated constructor stub

}

void MotionSpot::handle()
{
	// input pin handling will notify on state change
	this->in.handle();
}

void MotionSpot::setup() {
	this->in.setup();
	this->in.setObserver(this);
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


