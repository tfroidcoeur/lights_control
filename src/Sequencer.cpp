/*
 * BlinkingLed.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "Sequencer.h"

#include <Arduino.h>
#include <ArduinoSTL.h>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include "logging.h"
#include <iostream>
using namespace std;


Sequencer::Sequencer(OutPin & out) :
		out(out), origvalue(0), pattern(nullptr), startTime(0), activeStep(0) {
}

Sequencer::~Sequencer() {
}

void Sequencer::stop(bool restore) {
	if (restore) {
		COUT_DEBUG(cout <<"stopping, back to value " << origvalue << endl);
		out.write(origvalue);
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

/* create a pattern from a string
 * sequence of <ms>*<value> separated by spaces*/
SeqPattern * Sequencer::createPattern(std::string pat){
	std::istringstream is(pat);
	std::list<SeqElement> elements;
	SeqPattern * result = new SeqPattern;
	std::string seqstr;

	while (std::getline(is, seqstr, ' ')) {
		char dummy;
		SeqElement el;
		std::istringstream is2(seqstr);
		is2 >> el.duration ;
		is2 >> dummy ; // read '*'
		is2 >> el.value ;
		elements.push_back(el);
	}
	SeqElement el = {0,-1};
	elements.push_back(el);

	result->elements = new SeqElement[elements.size()];
	copy(elements.begin(), elements.end(), result->elements);

	result->repeatcount = 1;

	return result;
}
