/*
 * OutPin.cpp
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
}

void OutPin::setup() {
	pinMode(this->id, OUTPUT);
	COUT_DEBUG( cout << "initialized output pin " <<  this->id << endl);
}

OutPin::~OutPin() {
	// TODO Auto-generated destructor stub
}

void OutPin::toggle() {
	write(!read());
}

void OutPin::write(int value) {
	COUT_DEBUG( cout << "write " << value << " to pin " << id << endl);
	digitalWrite(this->id, value);
}

int OutPin::read() {
	return digitalRead(this->id);
}
