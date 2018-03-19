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


class Teleruptor: public Actor, public sigslot::has_slots<> {
public:
	Teleruptor(InPin & inpin , OutPin & outpin);
	virtual ~Teleruptor();
	virtual void handle(void);
	virtual void setup(void);
	void notifyInPin(int value);
private:
	InPin &in;
	OutPin &out;
};

#endif /* TELERUPTOR_H_ */
