/*
 * inpin.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */


#include "Arduino.h"
#include "ArduinoSTL.h"
#include "InPin.h"
//#define DEBUG
#undef DEBUG
#include "logging.h"
using namespace std;

bool InPin::read() {
	return digitalRead(id) == HIGH;
}

InPin::InPin(int id): id(id){
}

void InPin::setup() {
	pinMode(this->id, INPUT);
	COUT_DEBUG(cout << "initialized input pin " << id << endl);
}

void InPin::handle() {
}
