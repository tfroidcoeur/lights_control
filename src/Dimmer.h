/*
 * Dimmer.h
 *
 *  Created on: Apr 2, 2018
 *      Author: fraco
 */

#ifndef DIMMER_H_
#define DIMMER_H_

#include "InPin.h"
#include "DebouncedInput.h"
#include "OutPin.h"
#include "PassThrough.h"
#include "Sequencer.h"
#include "Switchable.h"
#include "MqttNode.h"
#include "sigslot.h"

class DimmerTracker : public sigslot::has_slots<>{
public:
  DimmerTracker(Input &in) : in(in){};
  void update(string const& path, string const & value);

  /* percent per second*/
  float dimSpeed;
  /* millis to press button before dim kicks in for off and on state */
  float dimThreshOnMs;
  float dimThreshOffMs;

  void updateInput(int val);

private:
  Input &in;
  unsigned long press_started;
};

class Dimmer: public Switchable, public Actor, public MqttNode {
public:
	Dimmer(Input * in, OutPin * outpin, string name, MqttNode * parent = NULL) ;
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
	bool laststate;
	OutPin & out;
	PassThrough passthrough;
	DebouncedInput debounced;
	Sequencer seq;
	DimmerTracker tracker;
	static SeqPattern * onSequence;
	static SeqPattern * offSequence;
	static SeqPattern * testSequence;
};

#endif /* DIMMER_H_ */
