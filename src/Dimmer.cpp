/*
 * Dimmer.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: fraco
 */

#include "Dimmer.h"
#define DEBUG
#include "DebouncedInput.h"
#include "logging.h"
#include <algorithm>
#include <iostream>
#include <string>

// min an max are defined as macros in Arduino.h
// clashing with cmath
#undef min
#undef max
#include <cmath>

Dimmer::Dimmer(Input *in, OutPin *outpin, const char *name, MqttNode *parent,
               float dimSpeed, float dimThreshOnMs, float dimThreshOffMs)
  : MqttNode(name, parent), out(*outpin), passthrough(*in, *outpin),
  debounced(outpin, false, 15, 250), seq(*outpin),
  tracker(*this, dimSpeed, dimThreshOnMs, dimThreshOffMs),
  controlling(false), synced(false), targetlvl(0.0) {}

Dimmer::~Dimmer() {
  debounced.getChangeSignal().disconnect_all();
}

/*
 * TODO
 * wanneer hij af staat:
 * x <= 400ms == aan
 * 400ms < x <900 ms lijkt geen effect (wschl super gedimd aan)
 * x >= 900ms soft voor kinderkamer (wschl al bij 400)
 * rond 400 doet ie raar: 395 < x < 405 brandt het licht maar lijkt hij toch in
 * off state

 * wanneer hij aan staat:
 * x >=900 ms dimmer threshold, dim =~ x-900, richting switch
 * x < 900 ms off
 *
 * minimum pulse is rond de 20ms
 */

/* long pulse then short to turn the dimmer lights off
   long pulse will turn the lights on in either state
   short pulse turns it off in on state */
SeqPattern *Dimmer::offSequence =
  new SeqPattern(1, new SeqElement[6] { { 100, false },
                                        { 1000, true },
                                        { 100, false },
                                        { 100, true },
                                        { 900, false },
                                        { 0, false } });

/* just pulse once. Beware: this will turn them off if they are on */
SeqPattern *Dimmer::onSequence = new SeqPattern(
  1,
  new SeqElement[4] { { 100, false }, { 100, true }, { 500, false },
                      { 0, false } });

/* change dim direction by dimming just minimum */
SeqPattern *Dimmer::dimDirSequence = new SeqPattern(
  1,
  new SeqElement[4] { { 100, false }, { 1000, true }, { 500, false },
                      { 0, false } });

/* stop any controls */
SeqPattern *Dimmer::stopSequence =
  new SeqPattern(1, new SeqElement[4] { { 500, false }, { 0, false } });

/* sync dimmer == reliably turn it on + dim to 100% */
SeqPattern *Dimmer::syncSequence =
  new SeqPattern(1, new SeqElement[12] { { 100, false },
                                         { 1000, true }, // on or dim slightly
                                         { 1000, false },
                                         { 100, true },  // off
                                         { 900, false },
                                         { 100, true },  // on again
                                         { 1000, false },
                                         { 1000, true }, // change dim direction
                                                         // to down
                                         { 1000, false },
                                         { 5000, true }, // dim off
                                         { 500, false },
                                         { 0, false } });

void Dimmer::on() {
  targeton = true;

  if (!controlling) targetlvl = tracker.dimlevel;
  controlling = true;
  checkSynced();
}

void Dimmer::off() {
  targeton = false;

  if (!controlling) targetlvl = tracker.dimlevel;
  controlling = true;

  /* force the off sequence, just in case we lost sync */
  if (seq.isRunning()) seq.stop();
  _off();
}

void Dimmer::_on() {
  if (!isOn()) {
    COUT_DEBUG(cout << "Starting dimmer on" << endl);
    passthrough.disable();
    seq.start(onSequence);
  }
  publish(string(name) + "/state", "ON");
}

void Dimmer::_off() {
  COUT_DEBUG(cout << "Starting dimmer off" << endl);
  passthrough.disable();
  seq.start(offSequence);
  publish(string(name) + "/state", "OFF");
}

bool Dimmer::isOn() {
  return tracker.isOn();
}

float Dimmer::getLevel() {
  if (!tracker.isOn()) {
    return 0.0f;
  }
  return tracker.getDimLevel();
}

bool Dimmer::targetStateReached() {
  return isOn() == targeton &&
         (!targeton ||
          std::abs(int(1000 * (tracker.getDimLevel() - targetlvl))) < 5);
}

/* Actor */
void Dimmer::handle() {
  if (!seq.isRunning()) {
    if (controlling) {
      COUT_DEBUG(float curlvl = tracker.getDimLevel());
      COUT_DEBUG(
        cout << "Controlling dimlevel target: " << targetlvl << " current: " <<
        curlvl);
      COUT_DEBUG(cout << " fabs diff: " <<
                 std::abs(int(1000 * (curlvl - targetlvl))) << endl);

      if (targetStateReached()) {
        COUT_DEBUG(cout << "target reached" << endl);
        controlling = false;
        seq.start(stopSequence);
      } else if (isOn() != targeton) {
        COUT_DEBUG(
          cout << "target on: " << targeton << " is now: " << isOn() << endl);

        if (targeton) _on();
        else {
          _off();
        }
      } else if ((targetlvl > tracker.getDimLevel()) != tracker.dimDirUp) {
        // change dim direction
        COUT_DEBUG(cout << "toggle dim direction " << tracker.dimDirUp << endl);
        passthrough.disable();
        seq.start(dimDirSequence);
      } else {
        if (!out.read()) {
          COUT_DEBUG(cout << "start dim dir: " << tracker.dimDirUp << endl);
          passthrough.disable();
          out.write(1);
        }
      }
    } else {
      passthrough.enable();
      passthrough.handle();
    }
  }

  seq.handle();
  debounced.handle();
}

void Dimmer::setup() {
  passthrough.setup();
  seq.setup();
  debounced.setup();
  debounced.getChangeSignal().connect(&tracker, &DimmerTracker::updateInput);
}

void Dimmer::dimCtrl(float lvl) {
  controlling = true;
  targeton    = true;
  targetlvl   = lvl;
}

void Dimmer::update(string const& path, string const& value) {
  COUT_DEBUG(cout << "update " << name << " " << path << " " << value << endl);

  if (path == "control") {
    if ((value == "ON") || (!isOn() && (value == "100"))) {
      on();
    } else if ((value == "OFF") || (isOn() && (value == "0"))) {
      off();
    } else {
      float lvlf = std::atof(value.c_str()) / 100.0;
      lvlf = std::min(std::max(lvlf, 0.0f), 100.0f);
      dimCtrl(lvlf);
    }
  } else if (path == "pulse") {
    int duration = std::atoi(value.c_str());
    unsigned long t;

    passthrough.disable();
    cout << "pulse...";
    out.write(0);

    t = millis() + 500;

    while (millis() < t) {
      debounced.handle();
      delay(1);
    }

    debounced.handle();
    out.write(1);
    debounced.handle();
    t = millis() + duration;

    while (millis() < t) {
      debounced.handle();
      delay(1);
    }
    debounced.handle();
    out.write(0);
    debounced.handle();
    cout << "done" << endl;
    passthrough.enable();
  } else {
    tracker.update(path, value);
  }
}

void Dimmer::publishDimLevel(float lvl) {
  char levelStr[10];

  // do not publish level 0 if dimmer is not off, because this confuses the
  // openhab client
  snprintf(levelStr, 10, "%d", int(lvl * 100));

  if (isOn() && (lvl < 0.01)) strcpy(levelStr, "0.01");
  COUT_DEBUG(cout << "report dimlevel " << lvl <<  " as " << levelStr << endl);
  publish(string(name) + "/dimlevel", string(levelStr));
}

void Dimmer::publishUpdate() {
  publish(string(name) + "/state", isOn() ? "ON" : "OFF");
  publishDimLevel(getLevel());
}

void Dimmer::refresh() {
  COUT_DEBUG(cout << "refresh " << name << endl);
  subscribe(string(name) + "/control");
  subscribe(string(name) + "/pulse");
  subscribe(string(name) + "/dimSpeed");
  subscribe(string(name) + "/dimThreshOn");
  subscribe(string(name) + "/dimThreshOff");
  publishUpdate();
}

void DimmerTracker::update(string const& path, string const& value) {
  COUT_DEBUG(cout << "tracker update " << " " << path << " " << value << endl);

  if (path == "dimSpeed") {
    dimSpeed = std::atof(value.c_str());
    COUT_DEBUG(cout << "dimSpeed: " << dimSpeed << endl);
  } else if (path == "dimThreshOff") {
    dimThreshOffMs = std::atof(value.c_str());
    COUT_DEBUG(cout << "dimThreshOffMs: " << dimThreshOffMs << endl);
  } else if (path == "dimThreshOn") {
    dimThreshOnMs = std::atof(value.c_str());
    COUT_DEBUG(cout << "dimThreshOnMs: " << dimThreshOnMs << endl);
  }
}

void DimmerTracker::updateInput(int val) {
  if (!val && lastval) {
    // if we toggle from 1 to 0 in stable val
    COUT_DEBUG(cout << "Dimmer "
                    << "pressed for " << millis() - press_started << "ms" <<
               endl);
    state->pulse(millis() - press_started);
    COUT_DEBUG(cout << string(isOn() ? "on" : "off") << " dir " <<
               string(
                 dimDirUp ? "up" : "down") << " lvl: " << getDimLevel() << endl);
    pressOngoing = false;
    dimmer.publishUpdate();
    dimmer.checkSynced();
  } else if (val && !lastval) {
    // toggle from 0 to 1
    press_started = millis();
    pressOngoing  = true;
  } else if (val) {
    // debouncedinput is repeating the stable high, update
    dimmer.publishDimLevel(calcNewDimLevel(millis() - press_started));
  }
  lastval = val;
}

float DimmerTracker::calcNewDimLevel(unsigned long duration) {
  float thresh   = isOn() ? dimThreshOnMs : dimThreshOffMs;
  float newlevel = dimlevel + (dimDirUp ? 1 : -1) * dimSpeed *
                   ((duration - thresh) / 1000.0);

  if (duration < thresh) return dimlevel;

  newlevel = min(max(newlevel, 0.0f), 1.0f);

  // COUT_DEBUG(cout << "calc lvl: base: " << dimlevel << " dirup: " << dimDirUp
  // << " duration: " << duration << " == " << newlevel << endl);
  return newlevel;
}

void DimmerTracker::changeState(DimmerState *newstate) {
  state = newstate;
}

void DimmerState_OFF::pulse(int duration) {
  // TODO soft on
  // TODO shady behaviour around threshold
  COUT_DEBUG(cout << "OFF:pulse " << duration << endl);
  tracker.dimDirUp = true;

  if (duration > tracker.dimThreshOffMs) {
    tracker.dimlevel = tracker.calcNewDimLevel(duration);
  }
  tracker.changeState(tracker.ON);
}

void DimmerState_ON::pulse(int duration) {
  COUT_DEBUG(cout << "ON:pulse " << duration << endl);

  if (duration > tracker.dimThreshOnMs) {
    tracker.dimlevel = tracker.calcNewDimLevel(duration);
    tracker.dimDirUp = !tracker.dimDirUp;
  } else {
    tracker.changeState(tracker.OFF);
  }
}

DimmerTracker::DimmerTracker(Dimmer& dimmer, float dimSpeed,
                             float dimThreshOnMs, float dimThreshOffMs)
  : dimSpeed(dimSpeed), dimThreshOnMs(dimThreshOnMs),
  dimThreshOffMs(dimThreshOffMs), dimlevel(0), dimDirUp(true),
  pressOngoing(false), OFF(new DimmerState_OFF(*this)),
  ON(new DimmerState_ON(*this)), state(NULL), dimmer(dimmer),
  lastval(false) {
  state = OFF;
}
