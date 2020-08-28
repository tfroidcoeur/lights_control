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

class Dimmer;
class DimmerTracker;

class DimmerState {
public:
	DimmerState(DimmerTracker & tracker): tracker(tracker) {};
    virtual ~DimmerState(){};
	virtual void pulse(int duration)=0;
protected:
	DimmerTracker& tracker;
};

class DimmerState_OFF: public DimmerState {
public:
	DimmerState_OFF(DimmerTracker & tracker) : DimmerState(tracker){ };
	virtual void pulse(int duration);
};

class DimmerState_ON: public DimmerState {
public:
	DimmerState_ON(DimmerTracker & tracker) : DimmerState(tracker){ };
	virtual void pulse(int duration);
};


class DimmerTracker : public sigslot::has_slots<>{
public:
  DimmerTracker(Input &in, Dimmer & dimmer, float dimSpeed=0.2, float dimThreshOnMs=400, float dimThreshOffMs=900);
  virtual ~DimmerTracker(){
	  delete OFF;
	  delete ON;
  }
  void update(string const& path, string const & value);
  void changeState(DimmerState * newstate);
  bool isOn() {return state== ON;};

  /* percent per second*/
  float dimSpeed;
  /* millis to press button before dim kicks in for off and on state */
  float dimThreshOnMs;
  float dimThreshOffMs;

  void updateInput(int val);

  DimmerState_OFF *OFF;
  DimmerState_ON *ON;
private:
  Input &in;
  unsigned long press_started;
  DimmerState * state;
  Dimmer & dimmer;
};

class Dimmer: public Switchable, public Actor, public MqttNode {
public:
	Dimmer(Input * in, OutPin * outpin, const char * name, MqttNode * parent = NULL) ;
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


	void publishUpdate();

private:
	bool isBlocked() { return seq.isRunning();}
	OutPin & out;
	PassThrough passthrough;
	DebouncedInput debounced;
	Sequencer seq;
	DimmerTracker tracker;
	static SeqPattern * onSequence;
	static SeqPattern * offSequence;

};

#endif /* DIMMER_H_ */
