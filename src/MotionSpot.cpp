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

MotionSpot::MotionSpot(int inid, int ctrlid, int forceid, int indicatorid) :
		in(inid), ctrl(ctrlid), force(forceid), indicator(indicatorid), button(
				in, &modes[0]), state(&MotionSpotState::Auto), blink(indicator) {
	button.setListener(this);
}

void MotionSpot::handle() {
	// button and input pin handling will notify on state change
	this->button.handle();
}

void MotionSpot::setup() {
	this->button.setup();
	this->ctrl.setup();
	this->force.setup();
}

void MotionSpot::notifyButton(Button & button, int mode) {
	MotionSpotState & nextState = state->next(mode);
	if (state != &nextState) {
		// change state
		state = &nextState;
		// notify by blinking
		blink.start(nextState.getPattern());
		// set outputs{ { 0, 1 }, { 5000, 0 }, { 5000, -1 }, }
		ctrl.write(nextState.getCtrl());
		force.write(nextState.getForce());
	}
}

BlinkPattern offpattern[] = { { 0, 1 }, { 400, 0 }, { 400, -2 }, };

BlinkPattern autopattern[] = { { 0, 1 }, { 1000, 0 }, { 0, -1 }, };

BlinkPattern onpattern[] = { {0, 1}, {0, -1 },};
MotionSpotState offstates[] = {MotionSpotState::Auto, MotionSpotState::ForcedOn};
MotionSpotState autostates[] = {MotionSpotState::ForcedOn, MotionSpotState::ForcedOn};
MotionSpotState onstates[] = {MotionSpotState::Off, MotionSpotState::Off};

static MotionSpotState MotionSpotState::Off(
		0,0, offpattern, offstates);
static MotionSpotState::Auto(
		0,0, offpattern, autostates);

static MotionSpotState::ForcedOn;

MotionSpotState::MotionSpotState(int force, int ctrl, BlinkPattern * pat,
		MotionSpotState (&nextstate)[]) :
		force(force), ctrl(ctrl), pat(*pat), nextstate(nextstate) {
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

