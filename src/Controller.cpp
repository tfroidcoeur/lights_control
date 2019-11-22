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
#include "MqttDirectory.h"


Controller::Controller(): buttonCA4(500,2000), buttonCA6(500,2000), buttonCA7(500,2000), buttonCA8(500,2000), buttonDM2(500,2000){
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

	// Mqtt directories
	huis = new MqttDirectory("home", NULL);
	// TODO create parent for huis: new class, light or controller?

	// Teleruptors
	teleruptorCA2 = new Teleruptor(inpinA[2], relay[2], "CA2", huis);
	teleruptorCC3 = new Teleruptor(inpinA[3], relay[3], "CA3", huis);
	teleruptorCA4 = new Teleruptor(buttonCA4.shortpress, relay[4], "CA4", huis);
	teleruptorDM1 = new Teleruptor(inpinA[5], relay[5], "DM1", huis);
	teleruptorCA6 = new Teleruptor(buttonCA6.shortpress, relay[6], "CA6", huis);
	teleruptorCA7 = new Teleruptor(buttonCA7.shortpress, relay[7], "CA7", huis);
	teleruptorCA8 = new Teleruptor(buttonCA8.shortpress, relay[8], "CA8", huis);
	teleruptorDM2 = new Teleruptor(buttonDM2.shortpress, relay[9], "DM2", huis);

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
	for(std::vector<InPin>::iterator it = inpinA.begin(); it != inpinA.end(); ++it) {
		it->changed.disconnect_all();
	}

	for(std::vector<InPin>::iterator it = inpinInt.begin(); it != inpinInt.end(); ++it) {
		it->changed.disconnect_all();
	}

	delete teleruptorCA2;
	delete teleruptorCC3;
	delete teleruptorCA4;
	delete teleruptorDM1;
	delete teleruptorCA6;
	delete teleruptorCA7;
	delete teleruptorCA8;
	delete teleruptorDM2;

	delete spotAA8;
	delete spotCC2;

	delete dimmerCB1;
	delete dimmerCB2;
}

void Controller::setupLivingGlobal(){
	living_off_actions.append(new FunAction<Dimmer>(dimmerCB1, &Dimmer::off));
	living_off_actions.append(new FunAction<Dimmer>(dimmerCB2, &Dimmer::off));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorDM1, &Teleruptor::save));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorDM1, &Teleruptor::off));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA4, &Teleruptor::save));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA4, &Teleruptor::off));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA6, &Teleruptor::save));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorCA6, &Teleruptor::off));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorDM2, &Teleruptor::save));
	living_off_actions.append(new FunAction<Teleruptor>(teleruptorDM2, &Teleruptor::off));

	buttonCA4.attach(inpinA[4].changed);
	buttonCA4.longpress.connect(&living_off_actions, &ActionList::doit);

	buttonCA6.attach(inpinA[6].changed);
	buttonCA6.longpress.connect(&living_off_actions, &ActionList::doit);

	buttonDM2.attach(inpinA[9].changed);
	buttonDM2.longpress.connect(&living_off_actions, &ActionList::doit);
}

void Controller::setupBureau(){
	bureau_off_actions.append(new FunAction<Teleruptor>(teleruptorCA7, &Teleruptor::off));
	bureau_off_actions.append(new FunAction<Teleruptor>(teleruptorCA8, &Teleruptor::off));

	buttonCA7.attach(inpinA[7].changed);
	buttonCA7.longpress.connect(&bureau_off_actions, &ActionList::doit);
	buttonCA8.attach(inpinA[8].changed);
	buttonCA8.longpress.connect(&bureau_off_actions, &ActionList::doit);
}

void Controller::setupGlobal(){
	global_off_actions.append(new FunAction<Dimmer>(dimmerCB1, &Dimmer::off));
	global_off_actions.append(new FunAction<Dimmer>(dimmerCB2, &Dimmer::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA2, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA2, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCC3, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCC3, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorDM1, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorDM1, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA4, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA4, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA6, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA6, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA7, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA7, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA8, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorCA8, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorDM2, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorDM2, &Teleruptor::off));

	buttonAA8.longpress.connect(&global_off_actions, &ActionList::doit);
}

void Controller::connectMotionSpot(MotionSpot & spot, sigslot::signal0<> * butshort, sigslot::signal0<> * butlong) {
	if (butshort) butshort->connect(&spot,&MotionSpot::shortpressed);
	if (butlong) butlong->connect(&spot,&MotionSpot::longpressed);
}

void Controller::setupMotionSpots() {
	buttonAA8.attach(inpinInt[1].changed);
	connectMotionSpot(*spotAA8, &buttonAA8.shortpress,NULL);
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
	setupGlobal();
	setupBureau();

	// connect the unused short press of CC2 to control the lamp CC3
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
	r.addActor(teleruptorDM1);
	r.addActor(teleruptorCA6);
	r.addActor(teleruptorCA7);
	r.addActor(teleruptorCA8);
	r.addActor(teleruptorDM2);

	vector<Dimmer>::iterator dimit;
	r.addActor(dimmerCB1);
	r.addActor(dimmerCB2);

	r.addActor(&buttonCA4);
	r.addActor(&buttonCA6);
	r.addActor(&buttonCA7);
	r.addActor(&buttonCA8);
	r.addActor(&buttonDM2);
	r.addActor(&buttonCB1);
	r.addActor(&buttonCB2);
	r.addActor(&buttonAA8);
	r.addActor(&buttonCC2);

	r.setup();
}

void Controller::handle() {
	r.handle();
}
