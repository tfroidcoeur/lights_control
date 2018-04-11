/*
 * Teleruptor.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#include "Teleruptor.h"
#include "Arduino.h"

//Teleruptor::Teleruptor(int inid, int outid):in(inid),out(outid) {
//	in.changed.connect(this, &Teleruptor::notifyInPin);
//}

Teleruptor::Teleruptor(InPin & inpin , OutPin & outpin):out(outpin) {
	inpin.changed.connect(this, &Teleruptor::notifyInPin);
}
Teleruptor::Teleruptor( sigslot::signal0<> & sig, OutPin & outpin):out(outpin) {
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

void Teleruptor::on() {
	out.write(1);

}
void Teleruptor::off() {
	out.write(0);
}

bool Teleruptor::isOn() {
	return out.read();
}

void Teleruptor::toggle() {
	out.toggle();
}
