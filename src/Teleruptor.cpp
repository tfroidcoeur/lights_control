/*
 * Teleruptor.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#include "Teleruptor.h"
#include "Arduino.h"

Teleruptor::Teleruptor(int inid, int outid):in(inid),out(outid) {
}

Teleruptor::~Teleruptor() {

}

void Teleruptor::handle() {
	// input pin handling will notify on state change
	this->in.handle();
}

void Teleruptor::setup() {
	this->in.setup();
	this->in.setObserver(this);
	this->out.setup();
}

void Teleruptor::notify(InPin * pin) {
	if (pin->getInPinValue() == HIGH) {
		// button pressed, toggle output
		this->out.toggle();
	}
}
