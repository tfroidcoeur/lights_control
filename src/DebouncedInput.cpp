/*
 * inpin.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */


#include "Arduino.h"
#include "ArduinoSTL.h"
#include "DebouncedInput.h"
//#define DEBUG
#undef DEBUG
#include "logging.h"
using namespace std;

int DebouncedInput::read() {
	return d.stableval;
}

int DebouncedInput::debounce() {
	unsigned long now = millis();
	int val = in->read();
	int previousval = d.readval;

	// transfer the value
	d.readval = val;

	unsigned long elapsed = now - d.changetime;
//	COUT_DEBUG(cout << "pin: " << in << " elapsed " << elapsed << endl);

	if (previousval != val) {
		// if changed from last read, reset the d timer
		d.changetime = now;
		COUT_DEBUG(cout << "debouncing input pin:" << in << endl);
	} else if (val != d.stableval && elapsed > debouncetime) {
		// new stable value
		COUT_DEBUG(cout << "dd input pin:" << in << " to value " << val << endl);
		d.stableval = val;
		return true;
	}
	return false;
}

DebouncedInput::DebouncedInput(Input *in, uint32_t debouncetime): debouncetime(debouncetime), in(in){
	// if someone is pressing a button during boot, it
	// will be dd and then reported as a buttonpush
	d.readval = d.stableval = 0;
	d.changetime = millis();
}

void DebouncedInput::handle() {
	if (debounce()) {
		// state changed after debouncing
		// call state change handler
		COUT_DEBUG(cout << "dd input pin:" << id << " emit " << hex << &changed << dec << endl);
		changed.emit(d.stableval);
	}
}
