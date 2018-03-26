/*
 * BlinkingLed.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "Sequencer.h"

#include "Controllino.h"
#include "logging.h"

Sequencer::Sequencer(OutPin & out) :
		out(out), origvalue(0), pattern(nullptr), startTime(0), activeStep(0) {
}

Sequencer::~Sequencer() {
}

void Sequencer::stop(bool restore) {
	if (restore) {
		COUT_DEBUG(cout <<"stopping, back to value " << origvalue << endl);
	} else {
		out.write(0);
	}
	pattern = nullptr;
}

void Sequencer::endPattern(int value) {

	// decrement positive repeatcounts
	if (repeatcount > 0)
		repeatcount--;

	if (repeatcount == 0) {
		COUT_DEBUG(Serial.println("stop repeat"));
		stop();
		return;
	}

	COUT_DEBUG(Serial.print("remaining repeatcount "));
	COUT_DEBUG(Serial.println(repeatcount));

	this->startTime = millis();
	this->activeStep = 0;
	activate();
}

void Sequencer::printStep(struct SeqElement & step) {
	COUT_DEBUG(cout << activeStep << "[" << step.duration << " ; " << step.value << "]" << endl);
}

void Sequencer::activate() {
	SeqElement & el = pattern->elements[activeStep];
	COUT_DEBUG(cout << "next step ");
	printStep(el);
	if (el.value == -1) {
		endPattern(el.value);
	} else {
		out.write(el.value);
	}
}

void Sequencer::handle() {
	if (!this->pattern)
		return;

	SeqElement & step = pattern->elements[activeStep];

	if ((unsigned long) millis() - this->startTime > step.duration) {
		COUT_DEBUG(cout << "" << millis()<< this->startTime<< ((unsigned long) millis() - this->startTime) <<endl);
		SeqElement & elold = pattern->elements[activeStep];
		COUT_DEBUG(cout << "executed step " );
		printStep(elold);
		startTime += elold.duration;
		activeStep++;
		activate();
	}
}

void Sequencer::start(struct SeqPattern * pattern) {
	this->pattern = pattern;
	this->startTime = millis();
	this->activeStep = 0;
	this->origvalue = out.read();
	this->repeatcount = pattern->repeatcount;
	activate();
	this->handle();
}