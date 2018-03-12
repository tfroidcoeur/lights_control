/*
 * MotionSpot.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef MOTIONSPOT_H_
#define MOTIONSPOT_H_

#include "BlinkingLed.h"
#include "Button.h"
#include "InPin.h"
#include "OutPin.h"

/* MotionSpot is a spotlight with motion detector control
 *
 * it includes a button, two controls and an indicator led
 * short press will cycle through the states
 * long press will switch between forced on and off
 * led will blink different patterns depending on mode
 */
class MotionSpotState {
public:
	virtual ~MotionSpotState();
	MotionSpotState(const char * name, int force, int ctrl, BlinkPattern &pat, MotionSpotState ** nextstate);

	MotionSpotState * next(int mode) {
		if (mode <1 || mode>2) return this;
		return nextstate[mode-1];
	}
	int getForce() { return force;}
	int getCtrl() {return ctrl;};
	const char * getName() {return name;};
	BlinkPattern * getPattern(){return &pat;};

	static MotionSpotState Off;
	static MotionSpotState Auto;
	static MotionSpotState ForcedOn;

private:
	int force;
	int ctrl;
	BlinkPattern (& pat);
	MotionSpotState ** nextstate;
	const char * name;

};

class MotionSpot: public Actor {
public:
	virtual ~MotionSpot();
	MotionSpot(int inid, int ctrlid, int forceid, int indicatorid);
	void setup();
	void handle();
	void shortpressed();
	void longpressed();


private:
	InPin in;
	OutPin ctrl;
	OutPin force;
	OutPin indicator;
	Button button;
	MotionSpotState * state;
	BlinkingLed blink;
	ButtonMode * modes;
	sigslot::signal0<> shortpress;
	sigslot::signal0<> longpress;
	void notifyButton(int mode);
};

#endif /* MOTIONSPOT_H_ */
