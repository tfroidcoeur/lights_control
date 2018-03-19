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

Teleruptor::Teleruptor(InPin & inpin , OutPin & outpin):in(inpin),out(outpin) {
	in.changed.connect(this, &Teleruptor::notifyInPin);
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
