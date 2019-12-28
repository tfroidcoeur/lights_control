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
#include "MqttNode.h"

class Dimmer: public Switchable, public Actor, public MqttNode {
public:
	Dimmer(InPin & inpin, OutPin & outpin, string name, MqttNode * parent = NULL) ;
	virtual ~Dimmer();

	/* Switchable */
	virtual void on();
	virtual void off();
	virtual bool isOn();

	/* Actor */
	virtual void handle();
	virtual void setup();

	/* mqtt node */
	virtual void update(string const& path, string const & value);
	virtual void refresh();
private:
	bool isBlocked() { return seq.isRunning();}
	Sequencer seq;
	PassThrough passthrough;
	OutPin & out;
	static SeqPattern * onSequence;
	static SeqPattern * offSequence;
	static SeqPattern * testSequence;
	bool laststate;
};

#endif /* DIMMER_H_ */
