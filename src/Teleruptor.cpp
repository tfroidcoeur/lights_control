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

//Teleruptor::Teleruptor(int inid, int outid):in(inid),out(outid) {
//	in.changed.connect(this, &Teleruptor::notifyInPin);
//}

Teleruptor::Teleruptor(InPin & inpin , OutPin & outpin):out(outpin), savedstate(false) {
	inpin.changed.connect(this, &Teleruptor::notifyInPin);
}
Teleruptor::Teleruptor( sigslot::signal0<> & sig, OutPin & outpin):out(outpin), savedstate(false) {
	sig.connect(this, &Teleruptor::toggle);
}

Teleruptor::~Teleruptor() {

}

void Teleruptor::handle() {
}

void Teleruptor::setup() {
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
}

void Teleruptor::off() {
	COUT_DEBUG(cout << "teleruptor off " << endl);
	out.write(0);
}

bool Teleruptor::isOn() {
	bool result= out.read();
	COUT_DEBUG(cout << "teleruptor is " << ( result?"on":"off" ) << endl);
	return result;
}

void Teleruptor::toggle() {
	COUT_DEBUG(cout << "teleruptor toggle " << endl);
	out.toggle();
}
