/*
 * Teleruptor.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */
#ifndef TELERUPTOR_H_
#define TELERUPTOR_H_

#include "InPin.h"
#include "OutPin.h"
#include "sigslot.h"
#include "Stashable.h"
#include "Switchable.h"


class Teleruptor: public Actor,
		public sigslot::has_slots<>,
		public Switchable,
		public Stashable {
public:
	Teleruptor(InPin & inpin, OutPin & outpin);
	Teleruptor(sigslot::signal0<> & sig, OutPin & outpin);
	virtual ~Teleruptor();
	virtual void handle(void);
	virtual void setup(void);
	void notifyInPin(int value);
// Switchable
	virtual void on();
	virtual void off();
	virtual bool isOn();
	virtual void toggle();
// Stashable
	virtual void save();
	virtual void restore();
private:
	OutPin &out;
	bool savedstate;
};

#endif /* TELERUPTOR_H_ */
