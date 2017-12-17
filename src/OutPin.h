/*
 * OutPin.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#ifndef OUTPIN_H_
#define OUTPIN_H_

class OutPin {
public:
	OutPin(int id);
	virtual ~OutPin();
	void setup();
	void handle(){};
	void toggle();
	void write(int val);
	int read();

private:
	int id;
};

#endif /* OUTPIN_H_ */
