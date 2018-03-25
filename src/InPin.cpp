/*
 * inpin.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */


#include "Arduino.h"
#include "InPin.h"
//#define DEBUG
#include "logging.h"

#define DEBOUNCETIME 20

int InPin::getInPinValue() {
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
	COUT_DEBUG(cout << "pin: " << id << " elapsed " << elapsed << endl);

	if (previousval != val) {
		// if changed from last read, reset the d timer
		this->d.changetime = now;
		COUT_DEBUG(cout << F("debouncing input pin:") << this->id << endl);
	} else if (val != this->d.stableval && elapsed > DEBOUNCETIME) {
		// new stable value
		COUT_DEBUG(cout << F("dd input pin:") << this->id << " to value " << val << endl);
		this->d.stableval = val;
		return true;
	}
	return false;
}

InPin::InPin(int id){
	this->id=id;
	// if someone is pressing a button during boot, it
	// will be dd and then reported as a buttonpush
	this->d.readval = this->d.stableval = 0;
	this->d.changetime = millis();
}

void InPin::setup() {
	pinMode(this->id, INPUT);
	Serial.print("initialized input pin ");
	Serial.println(this->id);
}

void InPin::handle() {
	if (this->debounce()) {
		// state changed after debouncing
		// call state change handler
		this->changed.emit(this->d.stableval);
	}
}
