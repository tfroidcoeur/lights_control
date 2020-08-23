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
#include <iostream>
#include <string>

Dimmer::Dimmer(Input * in, OutPin * outpin, string name, MqttNode * parent) :
		MqttNode(name, parent), laststate(false), out(*outpin),
		passthrough(*in,*outpin), debounced(in, false), seq(*outpin), tracker(*in, *this)
{
}

Dimmer::~Dimmer() {
}

/*
 * TODO
 * wanneer hij af staat:
 * x <= 400ms == aan
 * 400ms < x <900 ms lijkt geen effect (wschl super gedimd aan)
 * x >= 900ms soft voor kinderkamer

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
		"200*1" "2000 * 0");

void Dimmer::on() {
	if (isBlocked()){
		COUT_DEBUG( cout << "Dimmer blocked" << endl);
		return;
	}

	if (!isOn()) {
		COUT_DEBUG( cout << "Starting dimmer on" << endl);
		laststate = true;
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
	laststate = false;
	passthrough.disable();
	seq.start(offSequence);
	publish(string(name)+"/state", "OFF");
}

bool Dimmer::isOn() {
	return this->tracker.isOn();
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
		passthrough.disable();
		cout << "pulse...";
		out.write(0);
		delay(500);
		out.write(1);
		delay(duration);
		out.write(0);
		cout << "done" << endl;
		passthrough.enable();
	} else {
		tracker.update(path, value);
	}

}

void Dimmer::publishUpdate(){
	publish(string(name)+"/state", isOn()?"ON":"OFF");
}

void Dimmer::refresh(){
	COUT_DEBUG(cout << "refresh " << name << endl);
	subscribe(string(name) + "/control");
	subscribe(string(name) + "/pulse");
	publishUpdate();
}

void DimmerTracker::update(string const &path, string const &value) {
  COUT_DEBUG(cout << "tracker update " << " " << path << " " << value << endl);

  if (path == "dimSpeed") {
    dimSpeed = std::atof(value.c_str());
  } else if (path == "dimThreshOff") {
    dimThreshOffMs = std::atof(value.c_str());
  } else if (path == "dimThreshOn") {
    dimThreshOnMs = std::atof(value.c_str());
  }
}

void DimmerTracker::updateInput(int val) {
  if (!val) {
    // if we toggle from 1 to 0 in stable val
    cout << "Dimmer "
         << "pressed for " << millis() - press_started << "ms" << endl;
	DimmerState* old = state;
	state->pulse(millis() - press_started);
	if (old != state) 
		dimmer.publishUpdate();
  } else {
    press_started = millis();
  }
}

void DimmerTracker::changeState(DimmerState &newstate) { state = &newstate; }

void DimmerState_OFF::pulse(int duration) { tracker.changeState(tracker.ON); }

void DimmerState_ON::pulse(int duration) {
  if (duration < tracker.dimThreshOffMs)
    tracker.changeState(tracker.OFF);
}

DimmerTracker::DimmerTracker(Input &in, Dimmer & dimmer, float dimSpeed, float dimThreshOnMs, float dimThreshOffMs) : 
	dimSpeed(dimSpeed), dimThreshOnMs(dimThreshOnMs) ,dimThreshOffMs(dimThreshOffMs),
	OFF(*new DimmerState_OFF(*this)), ON(*new DimmerState_ON(*this)), in(in), state(&OFF), dimmer(dimmer) {};
