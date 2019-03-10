/*
 * OutPin.cpp
 *
 * Simple abstraction of an digital output pin.
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#include <Arduino.h>
#include <ArduinoSTL.h>
#include "OutPin.h"
//#define DEBUG
#include "logging.h"
using namespace std;

OutPin::OutPin(int id) {
	this->id = id;
	this->state=0;
}

void OutPin::setup() {
	pinMode(this->id, OUTPUT);
	this->state=digitalRead(this->id);
	COUT_DEBUG( cout << "initialized output pin " <<  this->id << endl);
}

OutPin::~OutPin() {
}

void OutPin::toggle() {
	write(!read());
}

void OutPin::write(int value) {
	COUT_DEBUG( cout << "write " << value << " to pin " << id << endl);
	this->state=value;
	digitalWrite(this->id, value);
}

int OutPin::read() {
	//return digitalRead(this->id);
	return this->state;
}
