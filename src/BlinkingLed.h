/*
 * BlinkingLed.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef BLINKINGLED_H_
#define BLINKINGLED_H_

#include "Actor.h"
#include "OutPin.h"

struct BlinkPattern {
	unsigned long delay;
	int value;
};

class BlinkingLed: public Actor {
public:
	BlinkingLed(OutPin & pin);
	virtual ~BlinkingLed();
	void setup() {
	}
	;
	void handle();
	void start(struct BlinkPattern * pattern, bool repeat = false);
	void start(struct BlinkPattern * pattern, int repeatcount );
	void stop();
private:
	OutPin & out;
	struct BlinkPattern * pattern;
	unsigned long startTime;
	int nextstep;
	int origvalue;
	int repeatcount;
	void endPattern(int value);
	void printStep(struct BlinkPattern & step);
};

#endif /* BLINKINGLED_H_ */
