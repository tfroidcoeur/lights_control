/*
 * MotionSpot.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "MotionSpot.h"
#include <limits.h>

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
	this->blink.handle();
}

void MotionSpot::setup() {
	this->button.setup();
	this->ctrl.setup();
	this->force.setup();
	this->blink.setup();
}

void MotionSpot::notifyButton(Button & button, int mode) {
	Serial.print("motionspot button mode ");
	Serial.print(mode);
	Serial.print(" state ");
	Serial.println(state->getName());
	MotionSpotState * nextState = state->next(mode);
	Serial.print("next state ");
	Serial.println(nextState->getName());
	if (state != nextState) {
		// change state
		state = nextState;
		// notify by blinking
		blink.stop();
		blink.start(nextState->getPattern());
		// set outputs{ { 0, 1 }, { 5000, 0 }, { 5000, -1 }, }
		ctrl.write(nextState->getCtrl());
		force.write(nextState->getForce());
	}
}

/* blink patterns of the led */
BlinkPattern offpattern = { .repeatcount = 3, .elements = (BlinkElement[] ) { {
				400, 1 }, { 400, 0 } } };

BlinkPattern autopattern = { .repeatcount = 1, .elements = (BlinkElement[] ) { {
				1000, 1 }, { 1000, 0 } } , };

// infinite repeat of very very long ON time
// it is even very unlikely that the ULONG_MAX will be detected, as we need
// to sample millis() exactly at the max before wrap around
BlinkPattern onpattern = { .repeatcount = -1, .elements = (BlinkElement[] ) { {
		ULONG_MAX, 1 }, { -1, 1 } } };

/* state changes on short or long button press*/

MotionSpotState* offstates[] =
		{ &MotionSpotState::Auto, &MotionSpotState::ForcedOn };
MotionSpotState* autostates[] = { &MotionSpotState::ForcedOn,
		&MotionSpotState::ForcedOn };
MotionSpotState* onstates[] = { &MotionSpotState::Off, &MotionSpotState::Off };

/* three states for the motionspot */
static MotionSpotState MotionSpotState::Off("Off", 0, 0, offpattern, offstates);
static MotionSpotState MotionSpotState::Auto("Auto",0, 1, autopattern, autostates);
static MotionSpotState MotionSpotState::ForcedOn("ForcedOn",1, 0, onpattern, onstates);

MotionSpotState::MotionSpotState(const char * name, int force, int ctrl, BlinkPattern & pat,
		MotionSpotState ** nextstate) :
		force(force), ctrl(ctrl), pat(pat), nextstate(nextstate), name(name) {
}

MotionSpotState::~MotionSpotState() {
}
