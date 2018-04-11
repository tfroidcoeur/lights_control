/*
 * Controller.cpp
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */

#include "Controller.h"

#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

#include "Button.h"
#include "Action.h"


Controller::Controller(){
	// create pins

	// inpinsA
	inpinA.push_back(InPin(CONTROLLINO_A0));
	inpinA.push_back(InPin(CONTROLLINO_A1));
	inpinA.push_back(InPin(CONTROLLINO_A2));
	inpinA.push_back(InPin(CONTROLLINO_A3));
	inpinA.push_back(InPin(CONTROLLINO_A4));
	inpinA.push_back(InPin(CONTROLLINO_A5));
	inpinA.push_back(InPin(CONTROLLINO_A6));
	inpinA.push_back(InPin(CONTROLLINO_A7));
	inpinA.push_back(InPin(CONTROLLINO_A8));
	inpinA.push_back(InPin(CONTROLLINO_A9));

	// inpin INT
	inpinInt.push_back(InPin(CONTROLLINO_IN0));
	inpinInt.push_back(InPin(CONTROLLINO_IN1));
	// outpins, relay
	relay.push_back(OutPin(CONTROLLINO_RELAY_00));
	relay.push_back(OutPin(CONTROLLINO_RELAY_01));
	relay.push_back(OutPin(CONTROLLINO_RELAY_02));
	relay.push_back(OutPin(CONTROLLINO_RELAY_03));
	relay.push_back(OutPin(CONTROLLINO_RELAY_04));
	relay.push_back(OutPin(CONTROLLINO_RELAY_05));
	relay.push_back(OutPin(CONTROLLINO_RELAY_06));
	relay.push_back(OutPin(CONTROLLINO_RELAY_07));
	relay.push_back(OutPin(CONTROLLINO_RELAY_08));
	relay.push_back(OutPin(CONTROLLINO_RELAY_09));

	// outpins, digital
	outpinD.push_back(OutPin(CONTROLLINO_D0));
	outpinD.push_back(OutPin(CONTROLLINO_D1));
	outpinD.push_back(OutPin(CONTROLLINO_D2));
	outpinD.push_back(OutPin(CONTROLLINO_D3));
	outpinD.push_back(OutPin(CONTROLLINO_D4));
	outpinD.push_back(OutPin(CONTROLLINO_D5));
	outpinD.push_back(OutPin(CONTROLLINO_D6));
	outpinD.push_back(OutPin(CONTROLLINO_D7));
	outpinD.push_back(OutPin(CONTROLLINO_D8));
	outpinD.push_back(OutPin(CONTROLLINO_D9));
	outpinD.push_back(OutPin(CONTROLLINO_D10));
	outpinD.push_back(OutPin(CONTROLLINO_D11));

	teleruptorCA2=new Teleruptor(inpinA[2], relay[2]);
	teleruptorCC3 = new Teleruptor(inpinA[3], relay[3]);
	teleruptorCA4=new Teleruptor(inpinA[4], relay[4]);
	teleruptor5 = new Teleruptor(inpinA[5], relay[5]);
	teleruptorCA6 = new Teleruptor(inpinA[6], relay[6]);
	teleruptorCA7 = new Teleruptor(inpinA[7], relay[7]);
	teleruptorCA8 = new Teleruptor(inpinA[8], relay[8]);
	teleruptorCA9 = new Teleruptor(inpinA[9], relay[9]);

	// Dimmers (passthrough)
	dimmers.push_back(Dimmer(inpinA[0], outpinD[10]));
	dimmers.push_back(Dimmer(inpinA[1], outpinD[11]));

	// create spots
	spot.push_back(
			MotionSpot(Controller::outpinD[0], Controller::outpinD[1],
					Controller::outpinD[2]));
	spot.push_back(
			MotionSpot(Controller::outpinD[3], Controller::outpinD[4],
					Controller::outpinD[5]));


//	cout << "constrcuted"<<endl;
}

Controller::~Controller(){
	// TODO: delete actions in global1actions
	// TODO: delete buttons

	delete teleruptorCA2;
	delete teleruptorCC3;
	delete teleruptorCA4;
	delete teleruptor5;
	delete teleruptorCA6;
	delete teleruptorCA7;
	delete teleruptorCA8;
	delete teleruptorCA9;
}


void Controller::setupLivingGlobal(){
	buttons.push_back(new SimpleButton());
	buttons.back()->attach(inpinA[4].changed);
	buttons.back()->longpress.connect(&global1actions, &ActionList::doit);

	buttons.push_back(new SimpleButton());
	buttons.back()->attach(inpinA[6].changed);
	buttons.back()->longpress.connect(&global1actions, &ActionList::doit);

	buttons.push_back(new SimpleButton());
	buttons.back()->attach(inpinA[9].changed);
	buttons.back()->longpress.connect(&global1actions, &ActionList::doit);

	global1actions.append(new FunAction<Dimmer>(&dimmers[0], &Dimmer::off));
	global1actions.append(new FunAction<Dimmer>(&dimmers[1], &Dimmer::off));
	global1actions.append(new FunAction<Teleruptor>(teleruptorCA4, &Teleruptor::off));
	global1actions.append(new FunAction<Teleruptor>(teleruptorCA6, &Teleruptor::off));
	global1actions.append(new FunAction<Teleruptor>(teleruptorCA9, &Teleruptor::off));

}

void Controller::connectMotionSpot(MotionSpot & spot, sigslot::signal0<> & butshort, sigslot::signal0<> & butlong) {
	butshort.connect(&spot,&MotionSpot::shortpressed);
	butlong.connect(&spot,&MotionSpot::longpressed);
}

void Controller::setupMotionSpots() {
	// FIXME: internal resize makes copies of the objects!!!!! MUST use pointers
	// create some buttons
	buttons.reserve(100);
	buttons.push_back(new SimpleButton());
	buttons.back()->attach(inpinInt[0].changed);
	connectMotionSpot(spot[0], buttons.back()->shortpress, buttons.back()->longpress);

	buttons.push_back(new SimpleButton());
	buttons.back()->attach(inpinInt[1].changed);
	connectMotionSpot(spot[1], buttons.back()->shortpress, buttons.back()->longpress);

	spot[0].setup();
	spot[1].setup();

	for (unsigned long i = 0; i < sizeof(spot) / sizeof(class MotionSpot); i++) {
		r.addActor(&spot[i]);
	}
}

void Controller::setup() {
	vector<InPin>::iterator it;
	setupMotionSpots();
	setupLivingGlobal();

	for (it = inpinA.begin(); it != inpinA.end(); it++) {
		r.addActor(&(*it));
	}
	for (it = inpinInt.begin(); it != inpinInt.end(); it++) {
		r.addActor(&(*it));
	}

	r.addActor(teleruptorCA2);
	r.addActor(teleruptorCC3);
	r.addActor(teleruptorCA4);
	r.addActor(teleruptor5);
	r.addActor(teleruptorCA6);
	r.addActor(teleruptorCA7);
	r.addActor(teleruptorCA8);
	r.addActor(teleruptorCA9);

	vector<Dimmer>::iterator dimit;
	for (dimit = dimmers.begin(); dimit!=dimmers.end() ; dimit++) {
		r.addActor(&(*dimit));
	}

	vector<SimpleButton *>::iterator butit;
	for (butit = buttons.begin(); butit!=buttons.end() ; butit++) {
		r.addActor(*butit);
	}

	r.setup();
}

void Controller::handle() {
	r.handle();
}

