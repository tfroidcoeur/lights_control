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


Controller::Controller(): buttonCA4(500,3000), buttonCA6(500,3000), buttonCA9(500,3000){
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
	teleruptorCA4=new Teleruptor(buttonCA4.shortpress, relay[4]);
	teleruptor5 = new Teleruptor(inpinA[5], relay[5]);
	teleruptorCA6 = new Teleruptor(buttonCA6.shortpress, relay[6]);
	teleruptorCA7 = new Teleruptor(inpinA[7], relay[7]);
	teleruptorCA8 = new Teleruptor(inpinA[8], relay[8]);
	teleruptorCA9 = new Teleruptor(buttonCA9.shortpress, relay[9]);

	// Dimmers (passthrough)
	dimmerCB1 = new Dimmer(inpinA[0], outpinD[10]);
	dimmerCB2 = new Dimmer(inpinA[1], outpinD[11]);

	// create spots
	spotAA8 = new MotionSpot(Controller::outpinD[3], Controller::outpinD[4],
					Controller::outpinD[5]);
	spotCC2 = new MotionSpot(Controller::outpinD[0], Controller::outpinD[1],
			Controller::outpinD[2]);
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

	delete spotAA8;
	delete spotCC2;

	delete dimmerCB1;
	delete dimmerCB2;
}

void Controller::setupLivingGlobal(){
	living_off_actions.append(new FunAction<Dimmer>(dimmerCB1, &Dimmer::off));
	living_off_actions.append(new FunAction<Dimmer>(dimmerCB2, &Dimmer::off));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA4, &Teleruptor::save));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA4, &Teleruptor::off));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA6, &Teleruptor::save));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA6, &Teleruptor::off));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA9, &Teleruptor::save));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA9, &Teleruptor::off));

	buttonCA4.attach(inpinA[4].changed);
	buttonCA4.longpress.connect(&living_off_actions, &ActionList::doit);

	buttonCA6.attach(inpinA[6].changed);
	buttonCA6.longpress.connect(&living_off_actions, &ActionList::doit);

	buttonCA9.attach(inpinA[9].changed);
	buttonCA9.longpress.connect(&living_off_actions, &ActionList::doit);

}

void Controller::connectMotionSpot(MotionSpot & spot, sigslot::signal0<> * butshort, sigslot::signal0<> * butlong) {
	if (butshort) butshort->connect(&spot,&MotionSpot::shortpressed);
	if (butlong) butlong->connect(&spot,&MotionSpot::longpressed);
}

void Controller::setupMotionSpots() {
	buttonAA8.attach(inpinInt[1].changed);
	connectMotionSpot(*spotAA8, &buttonAA8.shortpress, &buttonAA8.longpress);
	spotAA8->setup();
	r.addActor(spotAA8);

	buttonCC2.attach(inpinInt[0].changed);
	connectMotionSpot(*spotCC2, &buttonCC2.longpress, NULL);
	spotCC2->setup();
	r.addActor(spotCC2);
}

void Controller::setup() {
	vector<InPin>::iterator init;
	vector<OutPin>::iterator outit;
	setupMotionSpots();
	setupLivingGlobal();

	// connect the unused short press of CC2 to
	buttonCC2.shortpress.connect(teleruptorCC3, &Teleruptor::pressed);

	for (init = inpinA.begin(); init != inpinA.end(); init++) {
		r.addActor(&(*init));
	}

	for (init = inpinInt.begin(); init != inpinInt.end(); init++) {
		r.addActor(&(*init));
	}

	for (outit = outpinD.begin(); outit != outpinD.end(); outit++) {
		r.addActor(&(*outit));
	}

	for (outit = relay.begin(); outit != relay.end(); outit++) {
		r.addActor(&(*outit));
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
	r.addActor(dimmerCB1);
	r.addActor(dimmerCB2);

	r.addActor(&buttonCA4);
	r.addActor(&buttonCA6);
	r.addActor(&buttonCA9);
	r.addActor(&buttonCB1);
	r.addActor(&buttonCB2);
	r.addActor(&buttonAA8);
	r.addActor(&buttonCC2);

	r.setup();
}

void Controller::handle() {
	r.handle();
}

