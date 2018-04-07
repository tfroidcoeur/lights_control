/*
 * Dimmer.h
 *
 *  Created on: Apr 2, 2018
 *      Author: fraco
 */

#ifndef DIMMER_H_
#define DIMMER_H_

#include "InPin.h"
#include "OutPin.h"
#include "PassThrough.h"
#include "Sequencer.h"
#include "Switchable.h"

class Dimmer: public Switchable, public Actor {
public:
	Dimmer(InPin & inpin, OutPin & outpin);
	virtual ~Dimmer();

	/* Switchable */
	virtual void on();
	virtual void off();
	virtual bool isOn();

	/* Actor */
	virtual void handle();
	virtual void setup();
private:
	Sequencer seq;
	PassThrough passthrough;
	static SeqPattern * onSequence;
	static SeqPattern * offSequence;
	bool laststate;
};

#endif /* DIMMER_H_ */
