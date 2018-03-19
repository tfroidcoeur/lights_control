/*
 * TestWithTime.h
 *
 *  Created on: Mar 17, 2018
 *      Author: fraco
 */

#ifndef TESTWITHTIME_H_
#define TESTWITHTIME_H_
#include <Time.h>

class TestWithTime {
public:
	TestWithTime();
	virtual ~TestWithTime();
protected:
	void setup();
	void tear_down();
protected:
	Time * time;
	Time * orig;
};

#endif /* TESTWITHTIME_H_ */
