/*
 * Dimmer.cpp
 *
 *  Created on: Apr 2, 2018
 *      Author: fraco
 */

#include "Dimmer.h"
// #define DEBUG
#include "logging.h"
#include <iostream>
#include <string>

Dimmer::Dimmer(InPin & inpin, OutPin & outpin, string name, MqttNode * parent) :
		MqttNode(name, parent), seq(outpin), passthrough(inpin,outpin),laststate(false), out(outpin)
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
		"100*0 1000*1 100*0 200*1");
/* just pulse once. Beware: this will turn them off if they are on */
SeqPattern * Dimmer::onSequence = Sequencer::createPattern(
		"200*1");

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
	publish(name+"/state", "ON");
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
	publish(name+"/state", "OFF");
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
}
void Dimmer::setup() {
	passthrough.setup();
	seq.setup();
}

void Dimmer::update(string const& path, string const & value){
	cout << "update " << name << " " << path << " " <<value <<endl;
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
	}


}

void Dimmer::refresh(){
	cout << "refresh " << name << endl;
	subscribe(name + "/control");
	subscribe(name + "/pulse");
	publish(name+"/state", isOn()?"ON":"OFF");
}
