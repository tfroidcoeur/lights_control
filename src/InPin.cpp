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

#define DEBOUNCETIME 20

int InPin::read() {
	return digitalRead(id);
}

int InPin::readStable() {
	return this->d.stableval;
}

int InPin::debounce() {
	unsigned long now = millis();
	// read the this
	int val = digitalRead(this->id);
	int previousval = this->d.readval;

	// transfer the value
	this->d.readval = val;

	unsigned long elapsed = now - this->d.changetime;
//	COUT_DEBUG(cout << "pin: " << id << " elapsed " << elapsed << endl);

	if (previousval != val) {
		// if changed from last read, reset the d timer
		this->d.changetime = now;
		COUT_DEBUG(cout << "debouncing input pin:" << this->id << endl);
	} else if (val != this->d.stableval && elapsed > DEBOUNCETIME) {
		// new stable value
		COUT_DEBUG(cout << "dd input pin:" << this->id << " to value " << val << endl);
		this->d.stableval = val;
		return true;
	}
	return false;
}

InPin::InPin(int id){
	this->id=id;
	// if someone is pressing a button during boot, it
	// will be dd and then reported as a buttonpush
	d.readval = d.stableval = 0;
	d.changetime = millis();
}

void InPin::setup() {
	pinMode(this->id, INPUT);
	COUT_DEBUG(cout << "initialized input pin " << id << endl);
}

void InPin::handle() {
	if (debounce()) {
		// state changed after debouncing
		// call state change handler
		COUT_DEBUG(cout << "dd input pin:" << id << " emit " << hex << &changed << dec << endl);
		changed.emit(this->d.stableval);
	}
}
