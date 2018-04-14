/*
 * PassThrough.h
 *
 *  Created on: Apr 7, 2018
 *      Author: fraco
 */

#ifndef PASSTHROUGH_H_
#define PASSTHROUGH_H_

#include <Arduino.h>
#include "InPin.h"
#include "OutPin.h"
#include "sigslot.h"
//#define DEBUG
#include "logging.h"

class PassThrough: public Actor, public sigslot::has_slots<> {
public:
	PassThrough(InPin & in, OutPin & out, bool enabled = true) :
			in(in), out(out), enabled(enabled) {
	}
	virtual ~PassThrough(){};
	void enable() {
		enabled = true;
	}
	void disable() {
		enabled = false;
	}

	virtual void handle(void){
		if (in.read() != out.read()) {
			int val = in.read();
			COUT_DEBUG( cout << F("passthrough copy ") << val << endl);
			out.write(val);
		}
	}
	virtual void setup(void){};
private:
	InPin & in;
	OutPin & out;
	bool enabled;
};

#endif /* PASSTHROUGH_H_ */
