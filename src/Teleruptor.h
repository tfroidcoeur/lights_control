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
#include "Observer.h"


class Teleruptor: public Observer {
public:
	Teleruptor(int inid, int outid);
	virtual ~Teleruptor();
	virtual void handle(void);
	virtual void setup(void);
	void notify(InPin * pin);
	void notify(Observable * pin) {
		// this can't really be good
		notify((InPin*) pin);
	}
private:
	InPin in;
	OutPin out;
};

#endif /* TELERUPTOR_H_ */
