/*
 * Dimmer.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: fraco
 */

#include "Dimmer.h"
// #define DEBUG
#include "DebouncedInput.h"
#include "logging.h"
#include <algorithm>
#include <iostream>
#include <string>

Dimmer::Dimmer(Input *in, OutPin *outpin, const char *name, MqttNode *parent,
               float dimSpeed, float dimThreshOnMs,
               float dimThreshOffMs)
    : MqttNode(name, parent), out(*outpin), passthrough(*in, *outpin),
      debounced(outpin, false, 300), seq(*outpin), tracker(*in, *this, dimSpeed, dimThreshOnMs, dimThreshOffMs) {}

Dimmer::~Dimmer() {
	debounced.getChangeSignal().disconnect_all();
}

/*
 * TODO
 * wanneer hij af staat:
 * x <= 400ms == aan
 * 400ms < x <900 ms lijkt geen effect (wschl super gedimd aan)
 * x >= 900ms soft voor kinderkamer (wschl al bij 400)
 * rond 400 doet ie raar: 395 < x < 405 brandt het licht maar lijkt hij toch in off state

 * wanneer hij aan staat:
 * x >=900 ms dimmer threshold, dim =~ x-900, richting switch
 * x < 900 ms off
 *
 * minimum pulse is rond de 20ms
 */

/* 1 sec on 100 ms off to turn the dimmer lights off */
SeqPattern * Dimmer::offSequence = Sequencer::createPattern(
		"100*0 1000*1 100*0 200*1 2000*0");
/* just pulse once. Beware: this will turn them off if they are on */
SeqPattern * Dimmer::onSequence = Sequencer::createPattern(
		"200*1 2000*0");

void Dimmer::on() {
	if (isBlocked()){
		COUT_DEBUG( cout << "Dimmer blocked" << endl);
		return;
	}

	if (!isOn()) {
		COUT_DEBUG( cout << "Starting dimmer on" << endl);
		passthrough.disable();
		seq.start(onSequence);
	}
	publish(string(name)+"/state", "ON");
}

void Dimmer::off() {
	if (isBlocked()){
		COUT_DEBUG( cout << "Dimmer blocked" << endl);
		return;
	}
	COUT_DEBUG( cout << "Starting dimmer off"<< endl);
	passthrough.disable();
	seq.start(offSequence);
	publish(string(name)+"/state", "OFF");
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

/* Actor */
void Dimmer::handle() {
	if (!seq.isRunning()) {
		passthrough.enable();
		passthrough.handle();
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

void Dimmer::update(string const& path, string const & value){
	COUT_DEBUG(cout << "update " << name << " " << path << " " <<value <<endl);
	if (path == "control") {
		if (value == "ON") {
			this->on();
		} else if (value == "OFF") {
			this->off();
		}
	} else if (path == "pulse") {
		int duration = std::atoi(value.c_str());
		unsigned long t;

		passthrough.disable();
		cout << "pulse...";
		out.write(0);

		t = millis()+500;
		while (millis() < t){
			debounced.handle();
			delay(1);
		}

		debounced.handle();
		out.write(1);
		debounced.handle();
		t = millis()+duration;
		while (millis() < t){
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

void Dimmer::publishUpdate() {
	char levelStr[10];
    publish(string(name) + "/state", isOn() ? "ON" : "OFF");

	snprintf(levelStr, 10, "%d.%02d", int(getLevel() * 100), int((getLevel() * 10000)) % 100);
    COUT_DEBUG(cout << "report dimlevel " << getLevel() <<  " as " << levelStr << endl);
    publish(string(name) + "/dimlevel", string(levelStr));
}

void Dimmer::refresh(){
	COUT_DEBUG(cout << "refresh " << name << endl);
	subscribe(string(name) + "/control");
	subscribe(string(name) + "/pulse");
	subscribe(string(name) + "/dimSpeed");
	subscribe(string(name) + "/dimThreshOn");
	subscribe(string(name) + "/dimThreshOff");
	publishUpdate();
}

void DimmerTracker::update(string const &path, string const &value) {
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
         << "pressed for " << millis() - press_started << "ms" << endl);
	state->pulse(millis() - press_started);
	dimmer.publishUpdate();
  } else if (val && !lastval) {
    // toggle from 0 to 1
    press_started = millis();
  } else if (val) {
    // debouncedinput is repeating the stable high, update
  }
  lastval=val;
}

float DimmerTracker::calcNewDimLevel(unsigned long duration){
	float thresh = isOn()?dimThreshOnMs:dimThreshOffMs;
	float newlevel = dimlevel + (dimDirUp ? 1 : -1) * dimSpeed * ((duration - thresh) / 1000.0);
    newlevel = min(max(newlevel, 0.0f), 1.0f);
	return newlevel;
}

void DimmerTracker::changeState(DimmerState * newstate) { state = newstate; }

void DimmerState_OFF::pulse(int duration) {
  // TODO soft on
  // TODO shady behaviour around threshold
  tracker.dimDirUp = true;
  if (duration > tracker.dimThreshOffMs) {
    tracker.dimlevel = tracker.calcNewDimLevel(duration);
  }
  tracker.changeState(tracker.ON);
}

void DimmerState_ON::pulse(int duration) {
  if (duration > tracker.dimThreshOnMs) {
    tracker.dimlevel = tracker.calcNewDimLevel(duration);
    tracker.dimDirUp = !tracker.dimDirUp;
  } else {
    tracker.changeState(tracker.OFF);
  }
}

DimmerTracker::DimmerTracker(Input &in, Dimmer &dimmer, float dimSpeed,
                             float dimThreshOnMs, float dimThreshOffMs)
    : dimSpeed(dimSpeed), dimThreshOnMs(dimThreshOnMs),
      dimThreshOffMs(dimThreshOffMs), dimlevel(0), dimDirUp(true),
      OFF(new DimmerState_OFF(*this)), ON(new DimmerState_ON(*this)), in(in),
      state(NULL), dimmer(dimmer), lastval(false) {
  state = OFF;
};
