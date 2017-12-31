/*
 * Button.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <Arduino.h>

#include "InPin.h"
#include "Observable.h"
#include "Observer.h"

class Button;

class ButtonListener {
public:
	virtual void notifyButton(Button & button, int mode)=0;
	virtual ~ButtonListener(){};

};

class Button: public Observer, public Actor {
public:
	Button(InPin & pin, unsigned long * t) :
			pin(pin), t(t), pending(false) {
		pin.setObserver(this);
	}
	virtual ~Button() {
		pin.setObserver(nullptr);
	}
	void setListener(ButtonListener * listener) {
		this->listener = listener;
	}
	virtual void notify(Observable * o) {
//		Serial.print("button notify ");
//		Serial.println(pin.getInPinValue());
		if (pin.getInPinValue()) {
			// went high
			started = millis();
			pending = true;
			mode = 0;
//			Serial.println("button started pending");
		} else if (pending) {
			pending = false;
//			Serial.println("button notification");
			listener->notifyButton(*this, mode);
		}
	}

	virtual void handle() {
		// handle pin, could call callbacks
		pin.handle();
//		Serial.print("button handler ");
//		Serial.print(pending);
//		Serial.print(" ");
//		Serial.print(millis());
//		Serial.print(" ");
//		Serial.print(started);
//		Serial.print(" ");
//		Serial.println(t);
		if (pending && listener && (millis() - started > t[mode])) {
			if (t[mode++] == 0) {
				pending = false;
//				Serial.print("button notification ");
//				Serial.println(mode);
				listener->notifyButton(*this, mode - 1);
			}
//			Serial.print("mode is ");
//			Serial.println(mode);
		}
	}
	virtual void setup() {
		pin.setup();
	}

private:
	InPin & pin;
	unsigned long * t;
	ButtonListener * listener;
	unsigned long started;bool pending = true;
	int mode;
};

#endif /* BUTTON_H_ */
