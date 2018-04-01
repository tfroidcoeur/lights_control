/*
 * Sequencer.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "Actor.h"
#include "OutPin.h"
#include <string>

/**
 * individual seq element
 * duration: duration in ms
 * value: value to set on the pin
 */
struct SeqElement {
	unsigned long duration;
	int value;
};

/**
 * a seq pattern is a series of seq elements plus some info
 */
struct SeqPattern {
	int repeatcount;
	struct SeqElement * elements;
};

/**
 * Blinking led makes an Outpin blink capable.
 * TODO: nicer to move functionality to BlinkPattern and provide an
 * Outpin to the start function - maybe
 */
class Sequencer: public Actor {
public:
	Sequencer(OutPin & pin);
	virtual ~Sequencer();
	/* functions for Actor */
	virtual void setup() {
	}
	virtual void handle();

	/* API */
	void start(struct SeqPattern * pattern);
	void stop(bool restore);
	void stop() {
		stop(true);
	}
	static SeqPattern * createPattern(std::string pat);
private:
	void activate();
	OutPin & out;
	int origvalue;
	struct SeqPattern * pattern;
	unsigned long startTime;
	int activeStep;
	int repeatcount;
	void endPattern(int value);
	void printStep(struct SeqElement & step);
};

#endif /* SEQUENCER_H_ */
