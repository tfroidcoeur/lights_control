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

class MotionSpot: public Actor, public sigslot::has_slots<> {
public:
	virtual ~MotionSpot();
	MotionSpot(const MotionSpot & orig): ctrl(orig.ctrl), force(orig.force), indicator(orig.indicator), state(orig.state), blink(orig.blink) {
//		cout << "mspto() " << hex << this << " from " << &orig << endl;
	}
	MotionSpot(OutPin & ctrl, OutPin & force, OutPin & indicator);
	void setup();
	void handle();
	void shortpressed();
	void longpressed();

private:
	void activateState();
	OutPin &ctrl;
	OutPin &force;
	OutPin &indicator;
	MotionSpotState * state;
	BlinkingLed blink;
	void notifyButton(int mode);
};

#endif /* MOTIONSPOT_H_ */
