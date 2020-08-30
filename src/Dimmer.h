/*
 * Dimmer.h
 *
 *  Created on: Apr 2, 2018
 *      Author: fraco
 */

#ifndef DIMMER_H_
#define DIMMER_H_

// #define DEBUG
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
	virtual void pressedOngoing(int duration)=0;
protected:
	DimmerTracker& tracker;
};

class DimmerState_OFF: public DimmerState {
public:
	DimmerState_OFF(DimmerTracker & tracker) : DimmerState(tracker){ };
	virtual void pulse(int duration);
	virtual void pressedOngoing(int duration){ };
};

class DimmerState_ON: public DimmerState {
public:
	DimmerState_ON(DimmerTracker & tracker) : DimmerState(tracker) { };
	virtual void pulse(int duration);
	virtual void pressedOngoing(int duration){
		
	};
};


class DimmerTracker : public sigslot::has_slots<>{
public:
  DimmerTracker(Dimmer & dimmer, float dimSpeed=0.2, float dimThreshOnMs=900, float dimThreshOffMs=400);
  virtual ~DimmerTracker(){
	  delete OFF;
	  delete ON;
  }
  void update(string const& path, string const & value);
  void changeState(DimmerState * newstate);
  bool isOn() {return state== ON;};
  float getDimLevel() { return calcNewDimLevel(pressOngoing?(millis()-press_started):0);}
  float calcNewDimLevel(unsigned long duration);

  /* percent per second*/
  /* speed/min |    min    |     1     |     2     |     3     |     4     |     5     |     6     |     7     |    max    |     
       min     |           |           |           |           |           |           |           |           |           |
       1       |           |           |           |           |           |           |           |           |           |
       2       |           |           |           |           |           |           |           |           |           |
       3       |           |           |           |           |           |           |           |           |           |
       4       |           |           |           |           |           |           |           |           |           |
       5       |           |   0.28    |           |           |           |           |           |           |           |
       6       |           |           |           |           |           |           |           |           |           |
       max     |           |           |           |           |           |           |           |           |           |
  */
  float dimSpeed;
  /* millis to press button before dim kicks in for off and on state */
  float dimThreshOnMs;
  float dimThreshOffMs;

  /* dimmer level state, kept here */
  float dimlevel;
  bool dimDirUp;
  bool pressOngoing;

  void updateInput(int val);

  DimmerState_OFF *OFF;
  DimmerState_ON *ON;
private:
  unsigned long press_started;
  DimmerState * state;
  Dimmer & dimmer;
  bool lastval;
};

class Dimmer: public Switchable, public Actor, public MqttNode {
public:
	Dimmer(Input * in, OutPin * outpin, const char * name, MqttNode * parent = NULL,
               float dimSpeed = 0.28, float dimThreshOnMs = 900,
               float dimThreshOffMs = 400) ;
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

	float getLevel();
  void dimCtrl(float lvl);
	void publishUpdate();
	void publishDimLevel(float lvl);

private:
	void _on();
	void _off();
	OutPin & out;
	PassThrough passthrough;
	DebouncedInput debounced;
	Sequencer seq;
	DimmerTracker tracker;
	static SeqPattern * onSequence;
	static SeqPattern * offSequence;
  static SeqPattern * dimDirSequence;
  static SeqPattern * stopSequence;
  bool controlling;
  bool targeton;
  float targetlvl;
};

#endif /* DIMMER_H_ */
