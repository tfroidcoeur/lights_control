/*
 * ControllerTest.h
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */

#ifndef CONTROLLERTEST_H_
#define CONTROLLERTEST_H_

#include <cpptest-suite.h>
#include <Controller.h>

#include "TestWithTime.h"

class ControllerTest: public Test::Suite, public TestWithTime {
public:
	ControllerTest();
	virtual ~ControllerTest();

	void setup();
	void tear_down();
	void advanceTimeAbit(int delay=100, int inc=10);

private:
	auto_ptr<Controller> controller;
	void testTeleruptors();
	void testMotionSpot();
};

#endif /* CONTROLLERTEST_H_ */
