/*
 * MotionSpot.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef MOTIONSPOT_H_
#define MOTIONSPOT_H_

#include "Button.h"
#include "InPin.h"
#include "OutPin.h"

class MotionSpot: public Actor, public ButtonListener {
public:
	virtual ~MotionSpot();
	MotionSpot(int inid, int ctrlid, int forceid, int indicatorid);
	MotionSpot(InPin & in, OutPin & ctrl, OutPin & force, OutPin & indicator);
	void setup();
	void handle();

private:
	InPin & in;
	Button & button;
	OutPin & force;
	OutPin & ctrl;
	OutPin & indicator;
	static unsigned long modes[];
};

#endif /* MOTIONSPOT_H_ */
