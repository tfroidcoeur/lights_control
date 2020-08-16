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
		passthrough(*in,*outpin), debounced(in, false), seq(*outpin), tracker(*in)
{
}

Dimmer::~Dimmer() {
}

/*
 * TODO
 * wanneer hij af staat:
 * x <= 400ms == aan
 * 400ms < x <1000 ms lijkt geen effect (wschl super gedimd aan)
 * x >= 1000ms soft voor kinderkamer

 * wanneer hij aan staat:
 * x >=1000 ms dimmer threshold, dim =~ x-1000, richting switch
 * x < 1000 ms off
 */

/* 1 sec on 100 ms off to turn the dimmer lights off */
SeqPattern * Dimmer::offSequence = Sequencer::createPattern(
		"100*0 1000*1 100*0 200*1 2000*0");
/* just pulse once. Beware: this will turn them off if they are on */
SeqPattern * Dimmer::onSequence = Sequencer::createPattern(
		"200*1" "2000 * 0");

SeqPattern * Dimmer::testSequence = NULL;

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
	/* worthless guess at possible state of the dimmer */
	return laststate;
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

void Dimmer::refresh(){
	COUT_DEBUG(cout << "refresh " << name << endl);
	subscribe(string(name) + "/control");
	subscribe(string(name) + "/pulse");
	publish(string(name)+"/state", isOn()?"ON":"OFF");
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
			cout << "Dimmer " << " " << "pressed for " << millis() - press_started << "ms" << endl;
		} else {
			press_started = millis();
		}
	}
