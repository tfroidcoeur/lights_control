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
#include "Switchable.h"


class Teleruptor: public Actor, public sigslot::has_slots<>, public Switchable {
public:
	Teleruptor(InPin & inpin , OutPin & outpin);
	virtual ~Teleruptor();
	virtual void handle(void);
	virtual void setup(void);
	void notifyInPin(int value);
// Switchable
	virtual void on();
	virtual void off();
	virtual bool isOn();
	virtual void toggle();
private:
	InPin &in;
	OutPin &out;
};

#endif /* TELERUPTOR_H_ */
