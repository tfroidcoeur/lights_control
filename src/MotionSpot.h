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

class MotionSpotState {
public:
	virtual ~MotionSpotState();
	MotionSpotState(int force, int ctrl, BlinkPattern *pat, MotionSpotState * nextstate);

	MotionSpotState & next(int mode) {
		if (mode <1 || mode>2) return *this;
		return nextstate[mode-1];
	}
	int getForce() { return force;}
	int getCtrl() {return ctrl;};
	BlinkPattern (&getPattern())[];

	static MotionSpotState Off;
	static MotionSpotState Auto;
	static MotionSpotState ForcedOn;

private:
	int force;
	int ctrl;
	BlinkPattern (& pat)[];
	MotionSpotState (&nextstate)[2];

};

class MotionSpot: public Actor, public ButtonListener {
public:
	virtual ~MotionSpot();
	MotionSpot(int inid, int ctrlid, int forceid, int indicatorid);
	void setup();
	void handle();
	virtual void notifyButton(Button & button, int mode);

private:
	InPin in;
	Button button;
	OutPin force;
	OutPin ctrl;
	OutPin indicator;
	BlinkingLed blink;
	static unsigned long modes[];
	MotionSpotState * state;
};

#endif /* MOTIONSPOT_H_ */
