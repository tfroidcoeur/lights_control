/*
 * MotionSpot.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "MotionSpot.h"
#include <limits.h>
#include "sigslot.h"

MotionSpot::~MotionSpot() {
	delete modes;
}

MotionSpot::MotionSpot(int inid, int ctrlid, int forceid, int indicatorid) :
		in(inid), ctrl(ctrlid), force(forceid), indicator(indicatorid), button(
				&modes[0]), state(&MotionSpotState::Auto), blink(indicator) {
			in.changed.connect(&button, &Button::pinChanged);
			modes = new ButtonMode[3];
			modes[0].delay=20;
			modes[0].pressed=&shortpress;
			modes[1].delay=2000;
			modes[1].pressed=&longpress;
			modes[2].delay=0;
			modes[2].pressed=NULL;
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
	this->indicator.setup();
	this->blink.setup();
}

void MotionSpot::shortpressed(){
	notifyButton(0);
}

void MotionSpot::longpressed(){
	notifyButton(1);
}

void MotionSpot::notifyButton(int mode) {
	Serial.print("motionspot button mode ");
	Serial.print(mode);
	Serial.print(" received in state ");
	Serial.println(state->getName());
	MotionSpotState * nextState = state->next(mode);
	Serial.print("new state ");
	Serial.println(nextState->getName());
	if (state != nextState) {
		// change state
		state = nextState;
		// notify by blinking
		blink.stop(false);
		blink.start(nextState->getPattern());
		// set outputs{ { 0, 1 }, { 5000, 0 }, { 5000, -1 }, }
		ctrl.write(nextState->getCtrl());
		force.write(nextState->getForce());
	}
}

/* blink patterns of the led */
BlinkPattern offpattern = { .repeatcount = 3, .elements = (BlinkElement[] ) { {
				300, 1 }, { 300, 0 }, { 0, -1 } } };

BlinkPattern autopattern = { .repeatcount = 0, .elements = (BlinkElement[] ) { {
				1000, 1 }, { 1000, 0 },{ 0, -1 } } , };

// infinite repeat of very very long ON time
// it is even very unlikely that the ULONG_MAX will be detected, as we need
// to sample millis() exactly at the max before wrap around
BlinkPattern onpattern = { .repeatcount = -1, .elements = (BlinkElement[] ) { {
		ULONG_MAX, 1 }, { 0, -1 } } };

/* state changes on short or long button press*/

MotionSpotState* offstates[] =
		{ &MotionSpotState::Auto, &MotionSpotState::ForcedOn };
MotionSpotState* autostates[] = { &MotionSpotState::ForcedOn,
		&MotionSpotState::ForcedOn };
MotionSpotState* onstates[] = { &MotionSpotState::Off, &MotionSpotState::Off };

/* three states for the motionspot */
MotionSpotState MotionSpotState::Off("Off", 0, 0, offpattern, offstates);
MotionSpotState MotionSpotState::Auto("Auto",0, 1, autopattern, autostates);
MotionSpotState MotionSpotState::ForcedOn("ForcedOn",1, 0, onpattern, onstates);

MotionSpotState::MotionSpotState(const char * name, int force, int ctrl, BlinkPattern & pat,
		MotionSpotState ** nextstate) :
		force(force), ctrl(ctrl), pat(pat), nextstate(nextstate), name(name) {
}

MotionSpotState::~MotionSpotState() {
}
