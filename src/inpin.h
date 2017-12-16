/*
 * inpin.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#ifndef INPIN_H_
#define INPIN_H_

struct Debounce {
	unsigned long changetime;
	int readval;
	int stableval;
};

class InPin {
private:
	int id;
	struct Debounce d;
	void * handlerdata;
	void (*handler)(struct InPin *, void * data);
public:
	InPin(int id);
	int getInPinValue();
	int debounce();
	void setHandler( void (*handler)(class InPin *, void * data), void * data);
	void handle();
};



#endif /* INPIN_H_ */
