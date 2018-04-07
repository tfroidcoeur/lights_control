/*
 * PassThrough.h
 *
 *  Created on: Apr 7, 2018
 *      Author: fraco
 */

#ifndef PASSTHROUGH_H_
#define PASSTHROUGH_H_

#include "InPin.h"
#include "OutPin.h"
#include "sigslot.h"

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
			out.write(in.read());
		}
	}
	virtual void setup(void){};
private:
	InPin & in;
	OutPin & out;
	bool enabled;
};

#endif /* PASSTHROUGH_H_ */
