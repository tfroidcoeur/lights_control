/*
 * inpin.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#ifndef INPIN_H_
#define INPIN_H_

#include "Actor.h"
#include "sigslot.h"
#include <stdint.h>

struct Debounce {
	unsigned long changetime;
	int readval;
	int stableval;
};

class InPin : public Actor {
private:
	int id;
	uint32_t debouncetime;
	struct Debounce d;
public:
	InPin(int id, uint32_t debouncetime=20);
	int read();
	int readStable();
	int debounce();
	virtual void handle();
	virtual void setup();
	sigslot::signal1<int> changed;
};



#endif /* INPIN_H_ */
