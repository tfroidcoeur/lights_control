/*
 * light.h
 *
 *  Created on: Mar 19, 2018
 *      Author: fraco
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <vector>

#include "Action.h"
#include "Dimmer.h"
#include "InPin.h"
#include "MotionSpot.h"
#include "OutPin.h"
#include "Runner.h"
#include "sigslot.h"
#include "SimpleButton.h"
#include "Teleruptor.h"
#include "MqttDirectory.h"
#include "MqttRoot.h"

class Controller: public Actor {
public:
	Controller();
	virtual ~Controller();
	void handle();
	void setup();
private:
	void setupMotionSpots();
	void connectMotionSpot(MotionSpot & spot, sigslot::signal0<> * butshort, sigslot::signal0<> * butlong);
	void setupLivingGlobal();
	void setupBureau();
	void setupGlobal();
	vector<InPin> inpinA;
	vector<OutPin> relay;
	vector<OutPin> outpinD;
	vector<InPin>inpinInt;

	Runner r;

	ActionList bureau_off_actions;
	ActionList living_off_actions;
	ActionList global_off_actions;

	// Teleruptors
	Teleruptor * teleruptorCA2;
	Teleruptor * teleruptorCC3;
	Teleruptor * teleruptorDM1;
	Teleruptor * teleruptorCA4;
	Teleruptor * teleruptorCA6;
	Teleruptor * teleruptorCA7;
	Teleruptor * teleruptorCA8;
	Teleruptor * teleruptorDM2;

	// buttons
	SimpleButton buttonCA4;
	SimpleButton buttonCA6;
	SimpleButton buttonCA7;
	SimpleButton buttonCA8;
	SimpleButton buttonDM2;
	SimpleButton buttonCB1;
	SimpleButton buttonCB2;

	SimpleButton buttonAA8;
	SimpleButton buttonCC2;

	// spots
	MotionSpot * spotAA8;
	MotionSpot * spotCC2;

	// dimmers
	Dimmer * dimmerCB1;
	Dimmer * dimmerCB2;

	// mqtt root
	MqttRoot mqtt;

	// MqttDirectories
	MqttDirectory * huis;
};





#endif /* CONTROLLER_H_ */
