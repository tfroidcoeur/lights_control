/*
 * SimpleButton.h
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */

#ifndef SIMPLEBUTTON_H_
#define SIMPLEBUTTON_H_

#include "Button.h"
#include "sigslot.h"
#include "InPin.h"

class SimpleButton: public Button {
public:
	SimpleButton(int shortdelay = 500, int longdelay = 1500) :
			Button(), shortdelay(shortdelay), longdelay(longdelay) {

		addMode(ButtonMode(shortdelay, "short", &shortpress));
		addMode(ButtonMode(longdelay, "long", &longpress));
	}

	SimpleButton(const SimpleButton & orig) :
			Button(), shortdelay(orig.shortdelay), longdelay(orig.longdelay) {
		addMode(ButtonMode(orig.shortdelay, "short", &shortpress));
		addMode(ButtonMode(orig.longdelay, "long", &longpress));
	}

	virtual ~SimpleButton() {

	}
	sigslot::signal0<> shortpress;
	sigslot::signal0<> longpress;
private:
	const int shortdelay;
	const int longdelay;
};

#endif /* SIMPLEBUTTON_H_ */
