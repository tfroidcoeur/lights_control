/*
 * BlinkingLed.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "Controllino.h"
#include "BlinkingLed.h"

BlinkingLed::BlinkingLed(OutPin & out) :
		out(out), origvalue(0), pattern(nullptr), startTime(0), activeStep(0) {
}

BlinkingLed::~BlinkingLed() {
}

void BlinkingLed::stop(bool restore=true) {
	if (restore) {
		Serial.print("stopping, back to value ");
		Serial.println(origvalue);
		out.write(origvalue);
	}
	pattern = nullptr;
}

void BlinkingLed::endPattern(int value) {
	if (repeatcount == 0) {
		Serial.println("stop repeat");
		stop();
		return;
	}

	// decrement positive repeatcounts
	if (repeatcount > 0)
		repeatcount--;

	Serial.print("remaining repeatcount ");
	Serial.println(repeatcount);

	this->startTime = millis();
	this->activeStep = 0;
	activate();
}

void BlinkingLed::printStep(struct BlinkElement & step) {
	Serial.print(activeStep);
	Serial.print("[");
	Serial.print(step.duration);
	Serial.print(" ; ");
	Serial.print(step.value);
	Serial.println("]");
}

void BlinkingLed::activate() {
	BlinkElement & el = pattern->elements[activeStep];
	Serial.print("next step ");
	printStep(el);
	if (el.value == -1) {
		endPattern(el.value);
	} else {
		out.write(el.value);
	}
}

void BlinkingLed::handle() {
	if (!this->pattern)
		return;

	BlinkElement & step = pattern->elements[activeStep];

	if ((unsigned long) millis() - this->startTime > step.duration) {
		Serial.println(millis());
		Serial.println(this->startTime);
		Serial.println((unsigned long) millis() - this->startTime);
		BlinkElement & elold = pattern->elements[activeStep];
		Serial.print("executed step ");
		printStep(elold);
		startTime += elold.duration;
		activeStep++;
		activate();
	}
}

void BlinkingLed::start(struct BlinkPattern * pattern) {
	this->pattern = pattern;
	this->startTime = millis();
	this->activeStep = 0;
	this->origvalue = out.read();
	this->repeatcount = pattern->repeatcount;
	activate();
	this->handle();
}
