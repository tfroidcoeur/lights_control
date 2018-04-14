/*
 * Button.h
 *
 * A button will monitor a pin (debounced output). It will define different modes, based on the duration of the pulse.
 * Each mode has a delay parameter. The button will emit an event with a mode if
 * the button was pressed for a duration of < mode.delay. It will emit the mode with the smallest delay value where
 * press duration < mode.delay.
 * If the button is pressed for a duration > any mode.delay, the mode with the largest mode.delay is emitted.
 *
 * example:
 *   mode1 delay=100
 *   mode2 delay=400
 *   mode3 delay=1000
 *
 *   for a press duration between
 *      0 and 100 -> mode 1
 *      100 and 400 --> mode 2
 *      400 and 1000 --> mode 3
 *
 *   after 1000 ms, mode 3 will be emitted anyways, even if button still pressed.
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Arduino.h>
#include <ArduinoSTL.h>
#include <HardwareSerial.h>
#include <stddef.h>
#include <iostream>
#include <set>
#include <utility>

#include "InPin.h"
#include "sigslot.h"
//#define DEBUG
#include "logging.h"

class Button;
using namespace std;

class ButtonMode {
public:
	ButtonMode(int delay=0, const char * name = "<anon>",
			sigslot::signal0<> * pressed = NULL) :
			name(name), delay(delay), pressed(pressed) {
	}
	const char * name;
	unsigned long delay;
	sigslot::signal0<> * pressed;

	bool operator <( const ButtonMode & other) const {
		return this->delay < other.delay;
	}

};
std::ostream &operator<<(std::ostream &os, const ButtonMode & m);

class Button: public sigslot::has_slots<>, public Actor {
public:
	Button() :
			started(0), pending(false) {
		curmode = modes.begin();
//		cout << "create but " << hex << this << dec<< endl;
	}

	Button(const Button & orig) : Button() {
		// copy the modes
		modes = orig.modes;

		// reset the curmode
		curmode=modes.begin();
//		cout << "copy but " << hex << &orig << dec << endl;
	}

	Button(InPin & p)  :
			started(0), pending(false) {
//		COUT_DEBUG(cout << " attach but " << hex << this << " to " << &p << dec << endl);
		curmode = modes.begin();
		p.changed.connect(this, &Button::pinChanged);
//		COUT_DEBUG(cout << "create but " << hex << this << dec << endl);
	}

	void attach(sigslot::signal1<int> & sig) {
		sig.connect(this, &Button::pinChanged);
//		COUT_DEBUG(cout << " attach but " << hex << this << " to " << dec << &sig << endl);
	}

	void addMode(const ButtonMode mode) {
		modes.insert(mode);
		curmode = modes.begin();
	}

	virtual ~Button() {
//		cout << "delete but " << hex << this << dec << endl;
		modes.clear();
	}

	void pinChanged(int value);

	virtual void setup() {
	}

	virtual void handle();

private:
	void emit(const ButtonMode & mode) const;

	std::set<ButtonMode> modes;
	std::set<ButtonMode>::iterator curmode;
	unsigned long started;bool pending = true;
};

#endif /* BUTTON_H_ */
