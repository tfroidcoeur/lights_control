/*
 * OutPin.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#include <Arduino.h>
#include "OutPin.h"

OutPin::OutPin(int id) {
	this->id = id;
}

void OutPin::setup() {
	pinMode(this->id, OUTPUT);
	Serial.print(F("initialized output pin "));
	Serial.println(this->id);
}

OutPin::~OutPin() {
	// TODO Auto-generated destructor stub
}

void OutPin::toggle() {
	write(!read());
}

void OutPin::write(int value) {
	Serial.print("write ");
	Serial.print(value);
	Serial.print(" to p ");
	Serial.println(this->id);
	digitalWrite(this->id, value);
}

int OutPin::read() {
	return digitalRead(this->id);
}
