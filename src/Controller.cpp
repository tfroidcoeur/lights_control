/*
 * Controller.cpp
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */
#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>
#include "Controller.h"
#include "Runner.h"
#include "SimpleButton.h"


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

	// teleruptors
//	teleruptors.push_back(Teleruptor(inpinA[0], relay[0]));
//	teleruptors.push_back(Teleruptor(inpinA[1], relay[1]));
	teleruptors.push_back(Teleruptor(inpinA[2], relay[2]));
	teleruptors.push_back(Teleruptor(inpinA[3], relay[3]));
	teleruptors.push_back(Teleruptor(inpinA[4], relay[4]));
	teleruptors.push_back(Teleruptor(inpinA[5], relay[5]));
	teleruptors.push_back(Teleruptor(inpinA[6], relay[6]));
	teleruptors.push_back(Teleruptor(inpinA[7], relay[7]));
	teleruptors.push_back(Teleruptor(inpinA[8], relay[8]));
	teleruptors.push_back(Teleruptor(inpinA[9], relay[9]));

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

	// create spot buttons
	spotButtons.push_back(SimpleButton());
	spotButtons.push_back(SimpleButton());
	spotButtons[0].attach(inpinInt[0].changed);
	spotButtons[1].attach(inpinInt[1].changed);

//	cout << "constrcuted"<<endl;
}

Controller::~Controller(){
}

void Controller::connectMotionSpot(MotionSpot & spot, sigslot::signal0<> & butshort, sigslot::signal0<> & butlong) {
	butshort.connect(&spot,&MotionSpot::shortpressed);
	butlong.connect(&spot,&MotionSpot::longpressed);
}

void Controller::setupMotionSpots() {
//	cout << "setup spots"<<endl;
//	cout << " connect " << hex << &spot[0] << " to " << hex << &spotButtons[0]<< endl;
	connectMotionSpot(spot[0], spotButtons[0].shortpress, spotButtons[0].longpress);
//	cout << " connect " << hex << &spot[1] << " to " << hex << &spotButtons[1]<< endl;
	connectMotionSpot(spot[1], spotButtons[1].shortpress, spotButtons[0].longpress);

	spot[0].setup();
	spot[1].setup();

	for (unsigned long i = 0; i < sizeof(spot) / sizeof(class MotionSpot); i++) {
		r.addActor(&spot[i]);
	}
//	cout << "setup spots done"<<endl;
}

void Controller::setup() {
//	cout << "setup"<<endl;
	vector<InPin>::iterator it;
	for (it = inpinA.begin(); it != inpinA.end(); it++) {
		r.addActor(&(*it));
	}
	for (it = inpinInt.begin(); it != inpinInt.end(); it++) {
		r.addActor(&(*it));
	}

	vector<Teleruptor>::iterator teit;
	for (teit = teleruptors.begin(); teit!=teleruptors.end() ; teit++) {
		r.addActor(&(*teit));
	}

	vector<SimpleButton>::iterator butit;
	for (butit = spotButtons.begin(); butit!=spotButtons.end() ; butit++) {
		r.addActor(&(*butit));
	}

	vector<Dimmer>::iterator dimit;
	for (dimit = dimmers.begin(); dimit!=dimmers.end() ; dimit++) {
		r.addActor(&(*dimit));
	}

	// TODO add pins and buttons
	setupMotionSpots();
	r.setup();
}

void Controller::handle() {
	r.handle();
}

