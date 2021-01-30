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
	Teleruptor * teleruptorAA2;
	Teleruptor * teleruptorAA5;
	Teleruptor * teleruptorBH2;
	Teleruptor * teleruptorEA;
	Teleruptor * teleruptorEB;

	// buttons
	SimpleButton buttonAA1;
	SimpleButton buttonAA2;
	SimpleButton buttonAA3;
	SimpleButton buttonAA5;
	SimpleButton buttonAA6;
	SimpleButton buttonAA8;
	SimpleButton buttonBH2;
	SimpleButton buttonEA;
	SimpleButton buttonEB;
	SimpleButton buttonEC2;


	// spots
	MotionSpot * spotEC2;

	// dimmers
	Dimmer * dimmerAA1;
	Dimmer * dimmerAA3;
	Dimmer * dimmerAA6;

	// mqtt root
	MqttRoot mqtt;

	// MqttDirectories
	MqttDirectory * huis;
};

Controller::Controller(): buttonAA1(500,2000), buttonAA2(500,2000), buttonAA3(500,2000), buttonAA5(500,2000), 
						  buttonAA6(500,2000), buttonAA8(500,4000), buttonEA(500,2000), buttonEB(500,2000), 
						  buttonEC2(500,2000), mqtt("Controllino2"){
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
	teleruptorAA2 = new Teleruptor(inpinA[2], outpinD[2], "AA2", huis);
	huis->addNode(teleruptorAA2);
	teleruptorAA5 = new Teleruptor(inpinA[5], outpinD[5], "AA5", huis);
	huis->addNode(teleruptorAA5);
	teleruptorBH2 = new Teleruptor(inpinA[9], outpinD[9], "BH2", huis);
	huis->addNode(teleruptorBH2);
	teleruptorEA = new Teleruptor(inpinA[0], outpinD[0], "EA", huis);
	huis->addNode(teleruptorEA);
	teleruptorEB = new Teleruptor(inpinA[4], outpinD[4], "EB", huis);
	huis->addNode(teleruptorEB);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "dimmers" << endl);
	// Dimmers (passthrough)
	dimmerAA1 = new Dimmer(inpinA[0]->getRawInput(), outpinD[1], "AA1", huis);
	huis->addNode(dimmerAA1);
	dimmerAA3 = new Dimmer(inpinA[0]->getRawInput(), outpinD[3], "AA3", huis);
	huis->addNode(dimmerAA3);
	dimmerAA6 = new Dimmer(inpinA[0]->getRawInput(), outpinD[6], "AA6", huis);
	huis->addNode(dimmerAA6);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "spots" << endl);
	// create spots
	spotEC2 = new MotionSpot(*outpinD[7], *outpinD[8],
					*outpinD[9], "EC2", huis);
	huis->addNode(spotEC2);
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

	delete teleruptorAA2;
	delete teleruptorAA5;
	delete teleruptorBH2;
	delete teleruptorEA;
	delete teleruptorEB;

	delete spotEC2;

	delete dimmerAA1;
	delete dimmerAA3;
	delete dimmerAA6;
}

void Controller::setupGlobal(){
	global_off_actions.append(new FunAction<Dimmer>(dimmerAA3, &Dimmer::off));
	global_off_actions.append(new FunAction<Dimmer>(dimmerAA6, &Dimmer::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorEA, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorEA, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorEB, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorEB, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorBH2, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorBH2, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorAA2, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorAA2, &Teleruptor::off));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorAA5, &Teleruptor::save));
	global_off_actions.append(new FunAction<Teleruptor>(teleruptorAA5, &Teleruptor::off));

	buttonAA8.getLongSignal().connect(&global_off_actions, &ActionList::doit);
}

void Controller::connectMotionSpot(MotionSpot & spot, sigslot::signal0<> * butshort, sigslot::signal0<> * butlong) {
	if (butshort) butshort->connect(&spot,&MotionSpot::shortpressed);
	if (butlong) butlong->connect(&spot,&MotionSpot::longpressed);
}

void Controller::setupMotionSpots() {
	buttonEC2.attach(inpinInt[1]->getChangeSignal());
	connectMotionSpot(*spotEC2, &buttonAA8.getShortSignal(),NULL);
	spotEC2->setup();
	r.addActor(spotEC2);
}

void Controller::setup() {
	vector<DebouncedInput*>::iterator init;
	vector<OutPin*>::iterator outit;
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Setup spots" << endl);
	setupMotionSpots();
	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Setup global" << endl);
	setupGlobal();

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
	r.addActor(teleruptorAA2);
	r.addActor(teleruptorAA5);
	r.addActor(teleruptorBH2);
	r.addActor(teleruptorEA);
	r.addActor(teleruptorEB);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Add actors dimmers" << endl);
	r.addActor(dimmerAA1);
	r.addActor(dimmerAA3);
	r.addActor(dimmerAA6);

	COUT_DEBUG(cout << "free: " << freeMemory() <<endl);
	COUT_DEBUG(cout << "Add actors buttons" << endl);
	r.addActor(&buttonAA1);
	r.addActor(&buttonAA2);
	r.addActor(&buttonAA3);
	r.addActor(&buttonAA5);
	r.addActor(&buttonAA6);
	r.addActor(&buttonAA8);
	r.addActor(&buttonEA);
	r.addActor(&buttonEB);
	r.addActor(&buttonEC2);
	r.addActor(&buttonBH2);

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