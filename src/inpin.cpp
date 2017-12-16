/*
 * inpin.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */


#include "inpin.h"
#include "Arduino.h"

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

	if (previousval != val) {
		// if changed from last read, reset the d timer
		this->d.changetime = now;
      Serial.print(F("debouncing input pin:"));
      Serial.print(this->id);
	} else if (val != this->d.stableval && elapsed > DEBOUNCETIME) {
		// new stable value
		Serial.print(F("dd input pin:"));
		Serial.print(this->id);
		Serial.print(F("to value "));
		Serial.print(val);
    Serial.println();
		this->d.stableval = val;
		return true;
	}
	return false;
}

InPin::InPin(int id){
	this->id=id;
	pinMode(this->id, INPUT);
	// if someone is pressing a button during boot, it
	// will be dd and then reported as a buttonpush
	this->d.readval = this->d.stableval = 0;
	this->d.changetime = millis();
	this->handlerdata = NULL;
	this->handler = NULL;
	Serial.print("initialized input pin ");
	Serial.print(this->id);

}

void InPin::setHandler( void (*handler)(class InPin *, void * data), void * data) {
	this->handlerdata = data;
	this->handler = handler;
}

void InPin::handle() {
	if (this->debounce() && this->handler) {
		// state changed after debouncing
		// call state change handler
		this->handler(this, this->handlerdata);
	}
}
