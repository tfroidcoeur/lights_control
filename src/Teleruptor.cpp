/*
 * Teleruptor.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#include "Teleruptor.h"
#include "Arduino.h"
#include "ArduinoSTL.h"
//#define DEBUG
#include "logging.h"
using namespace std;

string const Teleruptor::ON = "ON";
string const Teleruptor::OFF = "ON";

Teleruptor::Teleruptor(InPin & inpin , OutPin & outpin, string name, MqttNode * parent):
		MqttNode(name, parent), out(outpin), savedstate(false) {
	inpin.changed.connect(this, &Teleruptor::notifyInPin);
}
Teleruptor::Teleruptor( sigslot::signal0<> & sig, OutPin & outpin, string name, MqttNode * parent):
		MqttNode(name, parent), out(outpin), savedstate(false) {
	sig.connect(this, &Teleruptor::toggle);
}

Teleruptor::~Teleruptor() {

}

void Teleruptor::handle() {
}

void Teleruptor::setup() {
	subscribe(name);
}

void Teleruptor::pressed() {
	notifyInPin(HIGH);
}

void Teleruptor::notifyInPin(int value) {
	if (value == HIGH) {
		// button pressed, toggle output
		this->out.toggle();
	}
}

void Teleruptor::save() {
	savedstate=out.read();
}

void Teleruptor::restore() {
	out.write(savedstate);
}

void Teleruptor::on() {
	COUT_DEBUG(cout << "teleruptor on " << endl);
	out.write(1);
	publish(name, ON);
}

void Teleruptor::off() {
	COUT_DEBUG(cout << "teleruptor off " << endl);
	out.write(0);
	publish(name, OFF);
}

bool Teleruptor::isOn() {
	bool result= out.read();
	COUT_DEBUG(cout << "teleruptor is " << ( result?"on":"off" ) << endl);
	return result;
}

void Teleruptor::toggle() {
	COUT_DEBUG(cout << "teleruptor toggle " << endl);
	out.toggle();
	publish(name, isOn() ? ON:OFF);
}

void Teleruptor::update(string const& path, string const & value){
	if (value == ON )
		on();
	else if (value == OFF)
		off();
}
