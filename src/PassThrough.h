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

class PassThrough: public Actor, public Input {
public:
	PassThrough(Input & in, OutPin & out, bool enabled = true) :
			in(in), out(out), enabled(enabled), val(0) {
	}
	virtual ~PassThrough(){};
	void enable() {
		enabled = true;
	}
	void disable() {
		enabled = false;
	}

	// return the result of the passthrough
	virtual bool read(){ return val;};

	virtual void handle(void){
		val = out.read();
		if (enabled && in.read() != val) {
			val = in.read();
			COUT_DEBUG( cout << "passthrough copy " << val << endl);
		} else if (!enabled) {
			val=0;
		}
		out.write(val);
	}

	virtual void setup(void){
		val=in.read();
	};

private:
	Input & in;
	OutPin & out;
	bool enabled;
	bool val;
};

#endif /* PASSTHROUGH_H_ */
