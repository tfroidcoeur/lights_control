/*
 * ControllerTest.cpp
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */

#include "ControllerTest.h"
#include "Controllino.h"
#include <cpptest-suite.h>
#include <cpptest-assert.h>

ControllerTest::ControllerTest(): controller(new Controller()) {
		TEST_ADD(ControllerTest::testTeleruptors);
}

ControllerTest::~ControllerTest() {
	// TODO Auto-generated destructor stub
}

void ControllerTest::setup(){
	TestWithTime::setup();
	controller=auto_ptr<Controller>(new Controller());
	controller->setup();
}

void ControllerTest::tear_down(){
	controller.release();
	TestWithTime::tear_down();
}

void ControllerTest::advanceTimeAbit(int delay, int inc){
	while (delay>0) {
		int step=inc>delay?delay:inc;
		*time+=step;
		delay-=step;
		controller->handle();
	}
}

void ControllerTest::testTeleruptors() {
	vector<int> inpins({
			CONTROLLINO_A0,
			CONTROLLINO_A1,
			CONTROLLINO_A2,
			CONTROLLINO_A3,
			CONTROLLINO_A4,
			CONTROLLINO_A5,
			CONTROLLINO_A6,
			CONTROLLINO_A7,
			CONTROLLINO_A8,
			CONTROLLINO_A9,
	});
	vector<int> outpins({
		CONTROLLINO_RELAY_00,
		CONTROLLINO_RELAY_01,
		CONTROLLINO_RELAY_02,
		CONTROLLINO_RELAY_03,
		CONTROLLINO_RELAY_04,
		CONTROLLINO_RELAY_05,
		CONTROLLINO_RELAY_06,
		CONTROLLINO_RELAY_07,
		CONTROLLINO_RELAY_08,
		CONTROLLINO_RELAY_09,
	});

	// the outputs should start low
	vector<int>::iterator it;
	for (it=outpins.begin(); it!=outpins.end(); it++) {
		TEST_ASSERT(!digitalRead(*it));
	}

	advanceTimeAbit(100);

	// push the button short
	for (it=inpins.begin(); it!=inpins.end(); it++) {
		digitalWrite(*it,1);
	}

	advanceTimeAbit(100);
	for (it=inpins.begin(); it!=inpins.end(); it++) {
		digitalWrite(*it,0);
	}

	advanceTimeAbit(100);

	// outputs should be high
	for (it=outpins.begin(); it!=outpins.end(); it++) {
		TEST_ASSERT(digitalRead(*it));
	}
}
