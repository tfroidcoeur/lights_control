/*
 * BlinkingLed.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "Controllino.h"
#include "BlinkingLed.h"

BlinkingLed::BlinkingLed(OutPin & out) :
		out(out), origvalue(0), pattern(nullptr), startTime(0), nextstep(0) {
}

BlinkingLed::~BlinkingLed() {
}

void BlinkingLed::stop() {
//	Serial.print("stopping, back to value ");
//	Serial.println(origvalue);
	pattern = nullptr;
	out.write(origvalue);
}

void BlinkingLed::endPattern(int value) {
	if (repeatcount==0) {
		stop();
		return;
	}

	// decrement positive repeatcounts
	if (repeatcount>0) repeatcount--;
	// transfer repeatcount from last value (negated) to create loops if repeatcount at -1
	else if (repeatcount == -1 && value !=-1) repeatcount=-value;
	this->startTime = millis();
	this->nextstep = 0;
}

void BlinkingLed::printStep(struct BlinkPattern & step) {
	Serial.print(nextstep);
	Serial.print("[");
	Serial.print(step.delay);
	Serial.print(" ; ");
	Serial.print(step.value);
	Serial.println("]");
}

void BlinkingLed::handle() {
	if (!this->pattern)
		return;

	BlinkPattern & step = this->pattern[this->nextstep];

	if ((unsigned long) millis() - this->startTime > step.delay) {
		if (step.value < 0) {
			endPattern(step.value);
			return;
		}
//		Serial.println(millis());
//		Serial.println(this->startTime);
//		Serial.println((unsigned long) millis() - this->startTime);
		startTime += step.delay;
		out.write(step.value);
//		Serial.print("execute step ");
//		printStep(step);
		nextstep++;
//		Serial.print("next step ");
//		printStep(pattern[nextstep]);
	}

	if (step.delay == 0 && step.value == -1) {
		endPattern(step.value);
		return;
	}
}

void BlinkingLed::start(struct BlinkPattern * pattern, bool repeat) {
	start(pattern,-1);
}

void BlinkingLed::start(struct BlinkPattern * pattern, int repeatcount) {
	this->pattern = pattern;
	this->startTime = millis();
	this->nextstep = 0;
	this->origvalue = out.read();
	this->repeatcount = repeatcount;
	this->handle();
}
