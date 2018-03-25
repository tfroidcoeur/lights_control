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
	SimpleButton(InPin & inpin, int shortdelay=20, int longdelay=1500) : Button(inpin)
	{

		addMode(ButtonMode(shortdelay, "short", &shortpress));
		addMode(ButtonMode(longdelay, "long", &longpress));
	}
	virtual ~SimpleButton(){

	}
	sigslot::signal0<> shortpress;
	sigslot::signal0<> longpress;
};

#endif /* SIMPLEBUTTON_H_ */
