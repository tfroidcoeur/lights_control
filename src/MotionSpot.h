/*
 * MotionSpot.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef MOTIONSPOT_H_
#define MOTIONSPOT_H_
#include "Actor.h"
#include "InPin.h"
#include "OutPin.h"

class MotionSpot : public Actor, public Observer {
public:
	virtual ~MotionSpot();
	MotionSpot(int inid, int ctrlid, int forceid);
	void setup();
	void handle();

private:
	InPin in;
	OutPin force;
	OutPin ctrl;
};

#endif /* MOTIONSPOT_H_ */
