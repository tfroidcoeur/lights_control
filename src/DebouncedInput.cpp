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
	return stableval;
}

int DebouncedInput::debounce() {
	unsigned long now = millis();
	int val = in->read();
	int previousval = readval;

	// transfer the value
	readval = val;

	unsigned long elapsed = now - changetime;
//	COUT_DEBUG(cout << "pin: " << in << " elapsed " << elapsed << endl);

	if (previousval != val) {
		// if changed from last read, reset the d timer
		changetime = now;
		COUT_DEBUG(cout << "debouncing input pin:" << in << endl);
	} else if (val != stableval && elapsed > debouncetime) {
		// new stable value
		COUT_DEBUG(cout << "dd input pin:" << in << " to value " << val << endl);
		stableval = val;
		return true;
	}
	return false;
}

DebouncedInput::DebouncedInput(Input *in, bool owninput, uint32_t debouncetime): 
	debouncetime(debouncetime), in(in), owninput(owninput){
	// if someone is pressing a button during boot, it
	// will be dd and then reported as a buttonpush
	readval = stableval = 0;
	changetime = millis();
}

void DebouncedInput::handle() {
	if (debounce()) {
		// state changed after debouncing
		// call state change handler
		COUT_DEBUG(cout << "dd input pin:" << in << " emit " << hex << &changed << dec << endl);
		changed.emit(stableval);
	}
}