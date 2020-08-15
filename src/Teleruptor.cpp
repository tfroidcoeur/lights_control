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
#include "Input.h"
#include "logging.h"
using namespace std;

string const Teleruptor::ON = "ON";
string const Teleruptor::OFF = "OFF";

Teleruptor::Teleruptor(NotifiedInput * inpin , OutPin * outpin, string name, MqttNode * parent):
		MqttNode(name, parent), out(*outpin), savedstate(false) {
	inpin->getChangeSignal().connect(this, &Teleruptor::notifyInPin);
}
Teleruptor::Teleruptor( sigslot::signal0<> & sig, OutPin * outpin, string name, MqttNode * parent):
		MqttNode(name, parent), out(*outpin), savedstate(false) {
	sig.connect(this, &Teleruptor::toggle);
}

Teleruptor::~Teleruptor() {

}

void Teleruptor::handle() {
}

void Teleruptor::setup() {
	refresh();
}

void Teleruptor::pressed() {
	notifyInPin(HIGH);
}

void Teleruptor::notifyInPin(int value) {
	if (value == HIGH) {
		// button pressed, toggle output
		this->toggle();
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
	publish(name+"/state", ON);
}

void Teleruptor::off() {
	COUT_DEBUG(cout << "teleruptor off " << endl);
	out.write(0);
	publish(name+"/state", OFF);
}

bool Teleruptor::isOn() {
	bool result= out.read();
	COUT_DEBUG(cout << "teleruptor is " << ( result?"on":"off" ) << endl);
	return result;
}

void Teleruptor::toggle() {
	COUT_DEBUG(cout << "teleruptor toggle " << endl);
	out.toggle();
	publish(name+"/state", isOn() ? ON:OFF);
}

void Teleruptor::update(string const& path, string const & value){
	COUT_DEBUG( cout << "teleruptor mqtt " << path << " " << value << endl);
	if (value == ON )
		on();
	else if (value == OFF)
		off();
}

void Teleruptor::refresh(){
	COUT_DEBUG( cout << "refresh " << name << endl);
	subscribe(name + "/control");
	// publish(name+"/state", isOn() ? ON:OFF);
}
