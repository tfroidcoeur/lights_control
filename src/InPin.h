/*
 * inpin.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#ifndef INPIN_H_
#define INPIN_H_

#include "Actor.h"
#include "Observable.h"
#include "sigslot.h"

struct Debounce {
	unsigned long changetime;
	int readval;
	int stableval;
};

class InPin : public Actor {
private:
	int id;
	struct Debounce d;
public:
	InPin(int id);
	int getInPinValue();
	int debounce();
	virtual void handle();
	virtual void setup();
	sigslot::signal1<int> changed;
};



#endif /* INPIN_H_ */
