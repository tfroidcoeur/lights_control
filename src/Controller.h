/*
 * light.h
 *
 *  Created on: Mar 19, 2018
 *      Author: fraco
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <vector>

#include "Dimmer.h"
#include "InPin.h"
#include "MotionSpot.h"
#include "OutPin.h"
#include "Runner.h"
#include "sigslot.h"
#include "SimpleButton.h"
#include "Teleruptor.h"

class Controller: public Actor {
public:
	Controller();
	virtual ~Controller();
	void handle();
	void setup();
private:
	void setupMotionSpots();
	void connectMotionSpot(MotionSpot & spot, sigslot::signal0<> & butshort, sigslot::signal0<> & butlong);
	vector<InPin> inpinA;
	vector<OutPin> relay;
	vector<OutPin> outpinD;
	vector<InPin>inpinInt;
	vector<Teleruptor> teleruptors;
	vector<MotionSpot> spot;
	vector<SimpleButton> spotButtons;
	vector<Dimmer> dimmers;
	Runner r;
};





#endif /* CONTROLLER_H_ */
