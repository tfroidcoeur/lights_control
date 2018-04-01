/*
 * TestWithTime.h
 *
 *  Created on: Mar 17, 2018
 *      Author: fraco
 */

#ifndef TESTWITHTIME_H_
#define TESTWITHTIME_H_
#include <Time.h>
#include <Actor.h>
#include <memory>

class TestWithTime {
public:
	TestWithTime();
	virtual ~TestWithTime();
	void advanceTimeAbit(int delay=100, int inc=10);
protected:
	void setup();
	void tear_down();
protected:
	Time * time;
	Time * orig;
protected:
	Actor * actor;
};

#endif /* TESTWITHTIME_H_ */
