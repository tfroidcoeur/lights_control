/*
 * MotionSpot.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "MotionSpot.h"
#include <limits.h>
#include "sigslot.h"

// #define DEBUG
#undef DEBUG
#include "logging.h"

MotionSpot::~MotionSpot() {
  //	cout << "~mspto " << hex << this <<endl;
}

// MotionSpot::MotionSpot(int ctrlid, int forceid, int indicatorid) :
//		ctrl(ctrlid), force(forceid), indicator(indicatorid), state(
//				&MotionSpotState::Auto), blink(indicator) {
// }

MotionSpot::MotionSpot(OutPin    & ctrl,
                       OutPin    & force,
                       OutPin    & indicator,
                       const char *name,
                       MqttNode   *parent) :
  MqttNode(name, parent), ctrl(&ctrl), force(&force), indicator(&indicator),
  state(
    &MotionSpotState::Auto), blink(indicator) {
  //	cout << "mspto " << hex << this <<endl;
}

void MotionSpot::handle() {
  // button and input pin handling will notify on state change
  blink.handle();
}

void MotionSpot::setup() {
  ctrl->setup();
  force->setup();
  indicator->setup();
  blink.setup();

  // reset all pins according to state
  activateState();
}

void MotionSpot::shortpressed() {
  notifyButton(1);
}

void MotionSpot::longpressed() {
  notifyButton(2);
}

void MotionSpot::activateState() {
  // notify by blinking
  blink.stop(false);
  blink.start(state->getPattern());

  // set outputs according to state
  ctrl->write(state->getCtrl());
  force->write(state->getForce());

  // mqtt notify
  publish(string(name) + "/state", state->getName());
}

void MotionSpot::notifyButton(int mode) {
  MotionSpotState *nextState = state->next(mode);

  COUT_DEBUG(
    cout << "motionspot button mode " << mode << " received in state " << state->getName() << " new state " << nextState->getName() <<
      endl);

  if (state != nextState) {
    // change state
    state = nextState;
    activateState();
  }
}

/* blink patterns of the led */
SeqPattern offpattern(3,
                      new SeqElement[3]  { { 300, true }, { 300, false }, { 0,
                                                                            false } });

SeqPattern autopattern(0,
                       new SeqElement[3]  { { 1000, true }, { 1000, false }, { 0,
                                                                               false } });

// infinite repeat of long ON time
// it is even very unlikely that the UINT16_MAX will be detected, as we need
// to sample millis() exactly at the max before wrap around
SeqPattern onpattern(-1,
                     new SeqElement[2] { { UINT16_MAX, true }, { 0, false } });

/* state changes on short or long button press*/

MotionSpotState *offstates[] = { &MotionSpotState::Auto,
                                 &MotionSpotState::ForcedOn };
MotionSpotState *autostates[] = { &MotionSpotState::ForcedOn,
                                  &MotionSpotState::ForcedOn };
MotionSpotState *onstates[] = { &MotionSpotState::Off, &MotionSpotState::Off };

/* three states for the motionspot */
MotionSpotState MotionSpotState::Off("OFF", 0, 0, offpattern, offstates);
MotionSpotState MotionSpotState::Auto("AUTO", 0, 1, autopattern, autostates);
MotionSpotState MotionSpotState::ForcedOn("ON", 1, 0, onpattern,
                                          onstates);

MotionSpotState::MotionSpotState(const char *name, int force, int ctrl,
                                 SeqPattern& pat, MotionSpotState **nextstate) :
  force(force), ctrl(ctrl), pat(pat), nextstate(nextstate), name(name) {}

MotionSpotState::~MotionSpotState() {}

void MotionSpot::activateState(MotionSpotState& newstate) {
  MotionSpotState *prevState = state;

  state = &newstate;

  if (prevState != state) activateState();
}

void MotionSpot::off() {
  activateState(MotionSpotState::Off);
}

void MotionSpot::update(string const& path, string const& value) {
  MotionSpotState *prevState = state;

  if (value ==
      MotionSpotState::ForcedOn.getName()) state = &MotionSpotState::ForcedOn;
  else if (value == MotionSpotState::Off.getName()) state = &MotionSpotState::Off;

  else if (value ==
           MotionSpotState::Auto.getName()) state = &MotionSpotState::Auto;

  if (prevState != state) activateState();
}

void MotionSpot::refresh() {
  cout << "refresh " << name << endl;
  subscribe(string(name) + "/control");
  publish(string(name) + "/state", state->getName());
}
