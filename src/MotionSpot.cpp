/*
 * MotionSpot.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "MotionSpot.h"
#include <limits.h>
#include "sigslot.h"
//#define DEBUG
#undef DEBUG
#include "logging.h"

MotionSpot::~MotionSpot() {
//	cout << "~mspto " << hex << this <<endl;
}

//MotionSpot::MotionSpot(int ctrlid, int forceid, int indicatorid) :
//		ctrl(ctrlid), force(forceid), indicator(indicatorid), state(
//				&MotionSpotState::Auto), blink(indicator) {
//}

MotionSpot::MotionSpot(OutPin & ctrl, OutPin & force, OutPin & indicator) :
		ctrl(ctrl), force(force), indicator(indicator), state(
				&MotionSpotState::Auto), blink(indicator) {
//	cout << "mspto " << hex << this <<endl;
}

void MotionSpot::handle() {
	// button and input pin handling will notify on state change
	blink.handle();
}

void MotionSpot::setup() {
	ctrl.setup();
	force.setup();
	indicator.setup();
	blink.setup();

	// reset all pins according to state
	activateState();
}

void MotionSpot::shortpressed() {
	notifyButton(1);
}

void MotionSpot::longpressed() {
	notifyButton(2);
}

void MotionSpot::activateState() {
	// notify by blinking
	blink.stop(false);
	blink.start(state->getPattern());

	// set outputs according to state
	ctrl.write(state->getCtrl());
	force.write(state->getForce());
}

void MotionSpot::notifyButton(int mode) {
	MotionSpotState * nextState = state->next(mode);
	COUT_DEBUG(cout << "motionspot button mode " << mode << " received in state " << state->getName() << " new state " << nextState->getName() <<endl);
	if (state != nextState) {
		// change state
		state = nextState;
		activateState();
	}
}

/* blink patterns of the led */
SeqPattern offpattern = { .repeatcount = 3, .elements = (SeqElement[] ) { {
				300, 1 }, { 300, 0 }, { 0, -1 } } };

SeqPattern autopattern = { .repeatcount = 0, .elements = (SeqElement[] ) { {
				1000, 1 }, { 1000, 0 }, { 0, -1 } } , };

// infinite repeat of very very long ON time
// it is even very unlikely that the ULONG_MAX will be detected, as we need
// to sample millis() exactly at the max before wrap around
SeqPattern onpattern = { .repeatcount = -1, .elements = (SeqElement[] ) { {
		ULONG_MAX, 1 }, { 0, -1 } } };

/* state changes on short or long button press*/

MotionSpotState* offstates[] = { &MotionSpotState::Auto,
		&MotionSpotState::ForcedOn };
MotionSpotState* autostates[] = { &MotionSpotState::ForcedOn,
		&MotionSpotState::ForcedOn };
MotionSpotState* onstates[] = { &MotionSpotState::Off, &MotionSpotState::Off };

/* three states for the motionspot */
MotionSpotState MotionSpotState::Off("Off", 0, 0, offpattern, offstates);
MotionSpotState MotionSpotState::Auto("Auto", 0, 1, autopattern, autostates);
MotionSpotState MotionSpotState::ForcedOn("ForcedOn", 1, 0, onpattern,
		onstates);

MotionSpotState::MotionSpotState(const char * name, int force, int ctrl,
		SeqPattern & pat, MotionSpotState ** nextstate) :
		force(force), ctrl(ctrl), pat(pat), nextstate(nextstate), name(name) {
}

MotionSpotState::~MotionSpotState() {
}
