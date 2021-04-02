/*
 * Controller.cpp
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */
#include <vector>
#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use
                            CONTROLLINO_xx aliases in your sketch. */

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
#include "Staircase.h"
#include "Teleruptor.h"

class Controller : public Actor {
public:

  Controller();
  virtual ~Controller();
  void handle();
  void setup();

private:

  void setupMotionSpots();
  void connectMotionSpot(MotionSpot        & spot,
                         sigslot::signal0<> *butshort,
                         sigslot::signal0<> *butlong);
  vector<DebouncedInput *>inpinA;
  vector<OutPin *>relay;
  vector<OutPin *>outpinD;
  vector<DebouncedInput *>inpinInt;

  Runner r;

  // Staircase
  Staircase *staircaseAA7;
  Staircase *staircaseCA1;

  // Teleruptors
  Teleruptor *teleruptorAA2;
  Teleruptor *teleruptorAA5;
  Teleruptor *teleruptorCC1;
  Teleruptor *teleruptorBH2;
  Teleruptor *teleruptorEA;
  Teleruptor *teleruptorEB;

  // buttons
  SimpleButton buttonEAEB;
  SimpleButton buttonAA2;
  SimpleButton buttonEC2;
  SimpleButton buttonAA5;
  SimpleButton buttonAA7;
  SimpleButton buttonCA1;


  // spots
  MotionSpot *spotEC2;

  // dimmers
  Dimmer *dimmerAA1;
  Dimmer *dimmerAA3;
  Dimmer *dimmerAA6;

  // mqtt root
  MqttRoot mqtt;

  // MqttDirectories
  MqttDirectory *huis;
};

Controller::Controller() : buttonEAEB(500, 2000), buttonAA2(500, 2000),
  buttonEC2(500, 2000), buttonAA5(500, 2000), buttonAA7(500, 2000),
  buttonCA1(500, 4000), mqtt("Controllino2") {
  // create pins

  COUT_DEBUG(cout << "size of DebouncedInput " << sizeof(DebouncedInput) << endl);
  COUT_DEBUG(cout << "size of SimpleButton " << sizeof(SimpleButton) << endl);
  COUT_DEBUG(cout << "size of Teleruptor " << sizeof(Teleruptor) << endl);
  COUT_DEBUG(cout << "size of FunAction<Teleruptor> " <<
             sizeof(FunAction<Teleruptor>) << endl);
  COUT_DEBUG(cout << "size of MqttRoot " << sizeof(MqttRoot) << endl);
  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "pins A" << endl);
  COUT_DEBUG(cout << "size of signal " << sizeof(sigslot::signal0<>) << endl);
  delay(1000);

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

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "pins INT" << endl);

  // inpin INT
  inpinInt.push_back(new DebouncedInput(new InPin(CONTROLLINO_IN0)));
  inpinInt.push_back(new DebouncedInput(new InPin(CONTROLLINO_IN1)));

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "pins relay" << endl);

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

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
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

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "huis" << endl);

  // Mqtt directories
  huis = new MqttDirectory("home", &mqtt);
  mqtt.setChild(huis);

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "teleruptors" << endl);

  // Buttons
  buttonEAEB.attach(inpinA[0]->getChangeSignal());
  buttonAA2.attach(inpinA[2]->getChangeSignal());
  buttonEC2.attach(inpinA[4]->getChangeSignal());
  buttonAA5.attach(inpinA[5]->getChangeSignal());
  buttonAA7.attach(inpinA[7]->getChangeSignal());
  buttonCA1.attach(inpinInt[0]->getChangeSignal());

  // Teleruptors
  teleruptorEA = new Teleruptor(inpinA[0], outpinD[0], "EA", huis);
  huis->addNode(teleruptorEA);
  teleruptorEB = new Teleruptor(inpinA[0], outpinD[2], "EB", huis);
  huis->addNode(teleruptorEB);
  teleruptorAA2 = new Teleruptor(inpinA[2], relay[2], "AA2", huis);
  huis->addNode(teleruptorAA2);
  teleruptorAA5 = new Teleruptor(inpinA[5], relay[5], "AA5", huis);
  huis->addNode(teleruptorAA5);
  teleruptorCC1 = new Teleruptor(inpinA[8], outpinD[8], "CC1", huis);
  huis->addNode(teleruptorCC1);
  teleruptorBH2 = new Teleruptor(inpinA[9], outpinD[9], "BH2", huis);
  huis->addNode(teleruptorBH2);

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "dimmers" << endl);

  // Staircases
  staircaseCA1 = new Staircase(*outpinD[10], "CA1", huis);
  huis->addNode(staircaseCA1);
  buttonCA1.getShortSignal().connect(staircaseCA1, &Staircase::shortpressed);
  buttonCA1.getLongSignal().connect(staircaseCA1, &Staircase::longpressed);

  staircaseAA7 = new Staircase(*relay[7], "AA7", huis);
  huis->addNode(staircaseAA7);
  buttonAA7.getShortSignal().connect(staircaseAA7, &Staircase::shortpressed);
  buttonAA7.getLongSignal().connect(staircaseAA7, &Staircase::longpressed);

  // Dimmers (passthrough)
  dimmerAA1 = new Dimmer(inpinA[1]->getRawInput(), outpinD[1], "AA1", huis);
  huis->addNode(dimmerAA1);
  dimmerAA3 = new Dimmer(inpinA[3]->getRawInput(), outpinD[3], "AA3", huis);
  huis->addNode(dimmerAA3);
  dimmerAA6 = new Dimmer(inpinA[6]->getRawInput(), outpinD[6], "AA6", huis);
  huis->addNode(dimmerAA6);

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "spots" << endl);

  // create spot
  // there is no LED indicator connected for the spot
  // but we must currently pass a pin for it
  spotEC2 = new MotionSpot(*outpinD[4], *outpinD[5],
                           *outpinD[7], "EC2", huis);
  huis->addNode(spotEC2);
  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
}

Controller::~Controller() {
  for (std::vector<DebouncedInput *>::iterator it = inpinA.begin();
       it != inpinA.end(); ++it) {
    delete *it;
  }

  for (std::vector<DebouncedInput *>::iterator it = inpinInt.begin();
       it != inpinInt.end(); ++it) {
    delete *it;
  }

  for (std::vector<OutPin *>::iterator it = relay.begin(); it != relay.end();
       ++it) {
    delete *it;
  }

  for (std::vector<OutPin *>::iterator it = outpinD.begin(); it != outpinD.end();
       ++it) {
    delete *it;
  }

  delete huis;

  delete teleruptorAA2;
  delete teleruptorAA5;
  delete teleruptorCC1;
  delete teleruptorBH2;
  delete teleruptorEA;
  delete teleruptorEB;
  delete staircaseCA1;
  delete staircaseAA7;

  delete spotEC2;

  delete dimmerAA1;
  delete dimmerAA3;
  delete dimmerAA6;
}

void Controller::connectMotionSpot(MotionSpot        & spot,
                                   sigslot::signal0<> *butshort,
                                   sigslot::signal0<> *butlong) {
  if (butshort) butshort->connect(&spot, &MotionSpot::shortpressed);

  if (butlong) butlong->connect(&spot, &MotionSpot::longpressed);
}

void Controller::setupMotionSpots() {
  connectMotionSpot(*spotEC2, &buttonEC2.getShortSignal(), NULL);
  spotEC2->setup();
  r.addActor(spotEC2);
  spotEC2->off();
}

void Controller::setup() {
  vector<DebouncedInput *>::iterator init;
  vector<OutPin *>::iterator outit;

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "Setup spots" << endl);
  setupMotionSpots();
  COUT_DEBUG(cout << "free: " << freeMemory() << endl);

  COUT_DEBUG(cout << "Add actors" << endl);
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

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "Add actors teleruptors" << endl);
  r.addActor(teleruptorAA2);
  r.addActor(teleruptorAA5);
  r.addActor(teleruptorBH2);
  r.addActor(teleruptorCC1);
  r.addActor(teleruptorEA);
  r.addActor(teleruptorEB);

  r.addActor(staircaseAA7);
  r.addActor(staircaseCA1);

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "Add actors dimmers" << endl);
  r.addActor(dimmerAA1);
  r.addActor(dimmerAA3);
  r.addActor(dimmerAA6);

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "Add actors buttons" << endl);
  r.addActor(&buttonAA2);
  r.addActor(&buttonAA5);
  r.addActor(&buttonCA1);
  r.addActor(&buttonEAEB);
  r.addActor(&buttonAA7);
  r.addActor(&buttonEC2);

  COUT_DEBUG(cout << "free: " << freeMemory() << endl);
  COUT_DEBUG(cout << "Run all actor setups" << endl);
  r.setup();
}

void Controller::handle() {
  r.handle();
}

Actor* createController() {
  return new Controller();
}
