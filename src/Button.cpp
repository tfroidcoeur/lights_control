/*
 * Button.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "Button.h"

std::ostream &operator<<(std::ostream &os, const ButtonMode & m) {
	return os << "mode " << (m.name ? m.name : "NULL") << "(" << m.delay << ","
			<< static_cast<void*>(m.pressed) << ")";
}

void Button::emit(const ButtonMode & mode) const {
	COUT_DEBUG(cout << "button " << hex << this << " notify: " << mode <<endl);
	if (mode.pressed)
		mode.pressed->emit();
}

void Button::handle() {
	if (!pending || curmode == modes.end()) {
		COUT_DEBUG(cout << "not pending, no handle" << endl);
		return;
	}
	// handle pin, could call callbacks
	COUT_DEBUG(cout << "button handler " << (pending?"":"not ") << "pending ms: " << millis() << " started " <<started <<endl); COUT_DEBUG(cout << "curmode: " << *curmode << endl);
	while (pending && (millis() - started > curmode->delay)) {
		const ButtonMode & prevmode = *curmode;
		if (++curmode == modes.end()) {
			pending = false;
			emit(prevmode);
			break;
		} COUT_DEBUG(cout << "next " << *curmode << endl);
	}
}

void Button::pinChanged(int value) {
	COUT_DEBUG(Serial.print("button notify ")); COUT_DEBUG(Serial.println(value));
	if (value) {
		// went high
		started = millis();
		pending = true;
		COUT_DEBUG(Serial.println("button started pending"));
	} else if (pending) {
		pending = false;
		if (curmode != modes.end())
			emit (*curmode);
	}
}
