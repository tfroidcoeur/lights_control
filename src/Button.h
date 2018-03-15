/*
 * Button.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Arduino.h>
#include <HardwareSerial.h>
#include <set>
#include <utility>
#include <iostream>

#include "Actor.h"
#include "sigslot.h"

class Button;
using namespace std;

class ButtonMode {
public:
	ButtonMode(int delay=0, const char * name = "<anon>",
			sigslot::signal0<> * pressed = NULL) :
			name(name), delay(delay), pressed(pressed) {
	}
	;
	const char * name;
	unsigned long delay;
	sigslot::signal0<> * pressed;bool operator <(
			const ButtonMode & other) const {
		return this->delay < other.delay;
	}

};
std::ostream &operator<<(std::ostream &os, const ButtonMode & m);

class Button: public sigslot::has_slots<>, public Actor {
public:
	Button() :
			started(0), pending(false) {
		curmode = modes.begin();
	}

	void addMode(const ButtonMode & mode) {
		modes.insert(mode);
		curmode = modes.begin();
	}

	virtual ~Button() {
		modes.clear();
	}

	void pinChanged(int value) {
		Serial.print("button notify ");
		Serial.println(value);
		if (value) {
			// went high
			started = millis();
			pending = true;
			Serial.println("button started pending");
		} else if (pending) {
			pending = false;
			if (curmode != modes.end()) emit(*curmode);
		}
	}

	virtual void setup() {
	}

	virtual void handle() {
		// handle pin, could call callbacks
		cout << "button handler " << (pending?"":"not ") << "pending ms: " << millis() << " started " <<started <<endl;
		cout << "curmode: " << *curmode << endl;
		while (pending && (millis() - started > curmode->delay)) {
			const ButtonMode & prevmode = *curmode;
			if (++curmode == modes.end()) {
				pending = false;
				emit(prevmode);
				break;
			}
			cout << "next " << *curmode << endl;
		}
	}

private:
	std::set<ButtonMode> modes;
	std::set<ButtonMode>::iterator curmode;
	unsigned long started;bool pending = true;
	void emit(const ButtonMode & mode) const {
		cout << "button notify: " << mode <<endl;
		if (mode.pressed)
			mode.pressed->emit();
	}
};

#endif /* BUTTON_H_ */
