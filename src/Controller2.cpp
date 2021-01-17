/*
 * Controller.cpp
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */
#include <vector>
#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */

#include "Action.h"
#include "Button.h"
#include "Controller.h"
#include "DebouncedInput.h"
#include "Dimmer.h"
#include "Input.h"
#include "MqttRoot.h"
#include "MqttDirectory.h"
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
	void connectMotionSpot(MotionSpot & spot, sigslot::signal0<> * butshort, sigslot::signal0<> * butlong);
	void setupLivingGlobal();
	void setupBureau();
	void setupGlobal();
	vector<DebouncedInput*> inpinA;
	vector<OutPin*> relay;
	vector<OutPin*> outpinD;
	vector<DebouncedInput*> inpinInt;

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
Controller::Controller(): buttonCA4(500,2000), buttonCA6(500,2000), buttonCA7(500,2000), buttonCA8(500,2000), buttonDM2(500,2000){
	// create pins

	COUT_DEBUG(cout << "size of DebouncedInput " << sizeof(DebouncedInput) << endl);
	COUT_DEBUG(cout << "size of SimpleButton " << sizeof(SimpleButton) << endl);
	COUT_DEBUG(cout << "size of Teleruptor " << sizeof(Teleruptor) << endl);
	COUT_DEBUG(cout << "size of FunAction<Teleruptor> " << sizeof(FunAction<Teleruptor>) << endl);
	COUT_DEBUG(cout << "size of MqttRoot " << sizeof(MqttRoot) << endl);
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "pins A" << endl);
	COUT_DEBUG(cout << "size of signal " << sizeof(sigslot::signal0<>) << endl);

	// inpinsA
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A0)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A1)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A2)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A3)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A4)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A5)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A6)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A7)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A8)));
	inpinA.push_back(new DebouncedInput(new InPin(CONTROLLINO_A9)));

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "pins INT" << endl);
	// inpin INT
	inpinInt.push_back(new DebouncedInput(new InPin(CONTROLLINO_IN0)));
	inpinInt.push_back(new DebouncedInput(new InPin(CONTROLLINO_IN1)));

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG( cout << "pins relay" << endl);
	// outpins, relay
	relay.push_back(new OutPin(CONTROLLINO_RELAY_00));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_01));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_02));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_03));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_04));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_05));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_06));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_07));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_08));
	relay.push_back(new OutPin(CONTROLLINO_RELAY_09));

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "pins D" << endl);
	// outpins, digital
	outpinD.push_back(new OutPin(CONTROLLINO_D0));
	outpinD.push_back(new OutPin(CONTROLLINO_D1));
	outpinD.push_back(new OutPin(CONTROLLINO_D2));
	outpinD.push_back(new OutPin(CONTROLLINO_D3));
	outpinD.push_back(new OutPin(CONTROLLINO_D4));
	outpinD.push_back(new OutPin(CONTROLLINO_D5));
	outpinD.push_back(new OutPin(CONTROLLINO_D6));
	outpinD.push_back(new OutPin(CONTROLLINO_D7));
	outpinD.push_back(new OutPin(CONTROLLINO_D8));
	outpinD.push_back(new OutPin(CONTROLLINO_D9));
	outpinD.push_back(new OutPin(CONTROLLINO_D10));
	outpinD.push_back(new OutPin(CONTROLLINO_D11));

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "huis" << endl);
	// Mqtt directories
	huis = new MqttDirectory("home", &mqtt);
	mqtt.setChild(huis);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG( cout << "teleruptors" << endl);
	// Teleruptors
	teleruptorCA2 = new Teleruptor(inpinA[2], relay[2], "CA2", huis);
	huis->addNode(teleruptorCA2);
	teleruptorCC3 = new Teleruptor(inpinA[3], relay[3], "CA3", huis);
	huis->addNode(teleruptorCC3);
	teleruptorCA4 = new Teleruptor(buttonCA4.getShortSignal(), relay[4], "CA4", huis);
	huis->addNode(teleruptorCA4);
	teleruptorDM1 = new Teleruptor(inpinA[5], relay[5], "DM1", huis);
	huis->addNode(teleruptorDM1);
	teleruptorCA6 = new Teleruptor(buttonCA6.getShortSignal(), relay[6], "CA6", huis);
	huis->addNode(teleruptorCA6);
	teleruptorCA7 = new Teleruptor(buttonCA7.getShortSignal(), relay[7], "CA7", huis);
	huis->addNode(teleruptorCA7);
	teleruptorCA8 = new Teleruptor(buttonCA8.getShortSignal(), relay[8], "CA8", huis);
	huis->addNode(teleruptorCA8);
	teleruptorDM2 = new Teleruptor(buttonDM2.getShortSignal(), relay[9], "DM2", huis);
	huis->addNode(teleruptorDM2);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "dimmers" << endl);
	// Dimmers (passthrough)
	dimmerCB1 = new Dimmer(inpinA[0]->getRawInput(), outpinD[10], "CB1", huis);
	huis->addNode(dimmerCB1);
	dimmerCB2 = new Dimmer(inpinA[1]->getRawInput(), outpinD[11], "CB2", huis);
	huis->addNode(dimmerCB2);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "spots" << endl);
	// create spots
	spotAA8 = new MotionSpot(*outpinD[3], *outpinD[4],
					*outpinD[5], "AA8", huis);
	huis->addNode(spotAA8);
	spotCC2 = new MotionSpot(*outpinD[0], *outpinD[1],
			*outpinD[2], "CC2", huis);
	huis->addNode(spotCC2);
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
}

Controller::~Controller(){
	for(std::vector<DebouncedInput*>::iterator it = inpinA.begin(); it != inpinA.end(); ++it) {
		delete *it;
	}

	for(std::vector<DebouncedInput*>::iterator it = inpinInt.begin(); it != inpinInt.end(); ++it) {
		delete *it;
	}

	for(std::vector<OutPin*>::iterator it = relay.begin(); it != relay.end(); ++it) {
		delete *it;
	}

	for(std::vector<OutPin*>::iterator it = outpinD.begin(); it != outpinD.end(); ++it) {
		delete *it;
	}

	delete huis;

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

	buttonCA4.attach(inpinA[4]->getChangeSignal());
	buttonCA4.getLongSignal().connect(&living_off_actions, &ActionList::doit);

	buttonCA6.attach(inpinA[6]->getChangeSignal());
	buttonCA6.getLongSignal().connect(&living_off_actions, &ActionList::doit);

	buttonDM2.attach(inpinA[9]->getChangeSignal());
	buttonDM2.getLongSignal().connect(&living_off_actions, &ActionList::doit);
}

void Controller::setupBureau(){
	bureau_off_actions.append(new FunAction<Teleruptor>(teleruptorCA7, &Teleruptor::off));
	bureau_off_actions.append(new FunAction<Teleruptor>(teleruptorCA8, &Teleruptor::off));

	buttonCA7.attach(inpinA[7]->getChangeSignal());
	buttonCA7.getLongSignal().connect(&bureau_off_actions, &ActionList::doit);
	buttonCA8.attach(inpinA[8]->getChangeSignal());
	buttonCA8.getLongSignal().connect(&bureau_off_actions, &ActionList::doit);
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

	buttonAA8.getLongSignal().connect(&global_off_actions, &ActionList::doit);
}

void Controller::connectMotionSpot(MotionSpot & spot, sigslot::signal0<> * butshort, sigslot::signal0<> * butlong) {
	if (butshort) butshort->connect(&spot,&MotionSpot::shortpressed);
	if (butlong) butlong->connect(&spot,&MotionSpot::longpressed);
}

void Controller::setupMotionSpots() {
	buttonAA8.attach(inpinInt[1]->getChangeSignal());
	connectMotionSpot(*spotAA8, &buttonAA8.getShortSignal(),NULL);
	spotAA8->setup();
	r.addActor(spotAA8);

	buttonCC2.attach(inpinInt[0]->getChangeSignal());
	connectMotionSpot(*spotCC2, &buttonCC2.getLongSignal(), NULL);
	spotCC2->setup();
	r.addActor(spotCC2);
}

void Controller::setup() {
	vector<DebouncedInput*>::iterator init;
	vector<OutPin*>::iterator outit;
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Setup spots" << endl);
	setupMotionSpots();
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Setup living" << endl);
	setupLivingGlobal();
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Setup global" << endl);
	setupGlobal();
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Setup bureau" << endl);
	setupBureau();
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Connect CC2" << endl);
	// connect the unused short press of CC2 to control the lamp CC3
	buttonCC2.getShortSignal().connect(teleruptorCC3, &Teleruptor::pressed);

	COUT_DEBUG( cout << "Add actors" << endl);
	r.addActor(&mqtt);
	for (init = inpinA.begin(); init != inpinA.end(); init++) {
		r.addActor(*init);
	}

	for (init = inpinInt.begin(); init != inpinInt.end(); init++) {
		r.addActor(*init);
	}

	for (outit = outpinD.begin(); outit != outpinD.end(); outit++) {
		r.addActor(*outit);
	}

	for (outit = relay.begin(); outit != relay.end(); outit++) {
		r.addActor(*outit);
	}

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Add actors teleruptors" << endl);
	r.addActor(teleruptorCA2);
	r.addActor(teleruptorCC3);
	r.addActor(teleruptorCA4);
	r.addActor(teleruptorDM1);
	r.addActor(teleruptorCA6);
	r.addActor(teleruptorCA7);
	r.addActor(teleruptorCA8);
	r.addActor(teleruptorDM2);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Add actors dimmers" << endl);
	r.addActor(dimmerCB1);
	r.addActor(dimmerCB2);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Add actors buttons" << endl);
	r.addActor(&buttonCA4);
	r.addActor(&buttonCA6);
	r.addActor(&buttonCA7);
	r.addActor(&buttonCA8);
	r.addActor(&buttonDM2);
	r.addActor(&buttonCB1);
	r.addActor(&buttonCB2);
	r.addActor(&buttonAA8);
	r.addActor(&buttonCC2);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Run all actor setups" << endl);
	r.setup();
}

void Controller::handle() {
	r.handle();
}

Actor* createController(){
	return new Controller();
}