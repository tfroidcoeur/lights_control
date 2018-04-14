/*
 * Dimmer.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: fraco
 */

#include "Dimmer.h"
//#define DEBUG
#include "logging.h"

Dimmer::Dimmer(InPin & inpin, OutPin & outpin) :
		seq(outpin), passthrough(inpin,outpin),laststate(false)  {
}

Dimmer::~Dimmer() {
}

/* 1 sec on 100 ms off to turn the dimmer lights off */
SeqPattern * Dimmer::offSequence = Sequencer::createPattern(
		"100*0 1000*1 100*0 200*1");
/* first turn the lights off, then single pulse to turn them on*/
SeqPattern * Dimmer::onSequence = Sequencer::createPattern(
		"100*0 1000*1 100*0 200*1 100*0 200*1");

void Dimmer::on() {
	COUT_DEBUG( cout << F("Starting dimmer on") << endl);
	laststate = true;
	passthrough.disable();
	seq.start(onSequence);
}
void Dimmer::off() {
	COUT_DEBUG( cout << F("Starting dimmer off")<< endl);
	laststate = false;
	passthrough.disable();
	seq.start(offSequence);
}
bool Dimmer::isOn() {
	/* worthless guess at possible state of the dimmer */
	return laststate;
}

/* Actor */
void Dimmer::handle() {
	if (!seq.isRunning()) {
		passthrough.enable();
		passthrough.handle();
	}
	seq.handle();
}
void Dimmer::setup() {
	passthrough.setup();
	seq.setup();
}
