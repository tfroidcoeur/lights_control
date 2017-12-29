/*
 * BlinkingLed.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef BLINKINGLED_H_
#define BLINKINGLED_H_

#include "Actor.h"
#include "OutPin.h"

/**
 * individual blink element
 * duration: duration in ms
 * value: value to set on the pin
 */
struct BlinkElement {
	unsigned long duration;
	int value;
};

/**
 * a blink pattern is a series of blink elements plus some info
 */
struct BlinkPattern {
	int repeatcount;
	struct BlinkElement * elements;
};

/**
 * Blinking led makes an Outpin blink capable.
 * TODO: nicer to move functionality to BlinkPattern and provide an
 * Outpin to the start function - maybe
 */
class BlinkingLed: public Actor {
public:
	BlinkingLed(OutPin & pin);
	virtual ~BlinkingLed();
	/* functions for Actor */
	virtual void setup() { } ;
	virtual void handle();

	/* API */
	void start(struct BlinkPattern * pattern);
	void stop(bool restore=true);
private:
	void activate();
	OutPin & out;
	struct BlinkPattern * pattern;
	unsigned long startTime;
	int activeStep;
	int origvalue;
	int repeatcount;
	void endPattern(int value);
	void printStep(struct BlinkElement & step);
};

#endif /* BLINKINGLED_H_ */
