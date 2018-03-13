/*
 * Button.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Arduino.h>
#include <list>

#include "Actor.h"
#include "Event.h"
#include "sigslot.h"

class Button;
using namespace std;

class ButtonMode{
public:
	unsigned long delay;
	sigslot::signal0<> * pressed;
	bool operator <(const ButtonMode & other) const
	{
		return this->delay < other.delay;
	}
};

class Button: public sigslot::has_slots<>, public Actor {
public:
	Button() :
			pending(false){
		curmode = modes.begin();
	}

	void addMode(const ButtonMode & mode){
		modes.insert(mode);
		curmode=modes.begin();
	}

	virtual ~Button() {
		modes.clear();
	}

	void pinChanged(int value) {
//		Serial.print("button notify ");
//		Serial.println(value);
		if (value) {
			// went high
			started = millis();
			pending = true;
			mode = 0;
//			Serial.println("button started pending");
		} else if (pending) {
			pending = false;
//			Serial.println("button notification");
			emit(mode);
		}
	}

	virtual void setup() { }

	virtual void handle() {
		// handle pin, could call callbacks
//		Serial.print("button handler ");
//		Serial.print(pending);
//		Serial.print(" ");
//		Serial.print(millis());
//		Serial.print(" ");
//		Serial.print(started);
//		Serial.print(" ");
//		Serial.println(t);
		if (pending && (millis() - started > curmode->delay)) {
			if (curmode->delay == 0) {
				pending = false;
//				Serial.print("button notification ");
//				Serial.println(mode);
				emit(mode);
			}
//			Serial.print("mode is ");
//			Serial.println(mode);
		}
	}

private:
	std::set<ButtonMode> modes;
	std::set<ButtonMode>::iterator curmode;
	unsigned long started;
	bool pending = true;
	int mode;
	void emit(int mode){
		if (curmode->pressed) curmode->pressed->emit();
	}
};

#endif /* BUTTON_H_ */
