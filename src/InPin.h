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

struct Debounce {
	unsigned long changetime;
	int readval;
	int stableval;
};

class InPin : public Actor, public Observable {
private:
	int id;
	struct Debounce d;
public:
	InPin(int id);
	int getInPinValue();
	int debounce();
	virtual void handle();
	virtual void setup();
};



#endif /* INPIN_H_ */
