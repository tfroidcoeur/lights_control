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
#include "Controller.h"

ControllerTest::ControllerTest(){
		TEST_ADD(ControllerTest::testMotionSpot);
		TEST_ADD(ControllerTest::testTeleruptors);
		TEST_ADD(ControllerTest::testGlobalButton);
}

ControllerTest::~ControllerTest() {
	// TODO Auto-generated destructor stub
}

void ControllerTest::setup(){
	TestWithTime::setup();
	actor=new Controller();
	pinReset();
	actor->setup();

	// skip the dimmer init
}

void ControllerTest::tear_down(){
	delete actor;
	TestWithTime::tear_down();
}

void ControllerTest::testTeleruptors() {
	vector<int> inpins({
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

void ControllerTest::testMotionSpot(){
	struct InPinInfo {
		int pinid;
		bool longpush;
	};
	vector<int>::iterator it;
	vector<struct InPinInfo>::iterator init;
	vector<struct InPinInfo> inpins({
			{ CONTROLLINO_IN0, true },
			{ CONTROLLINO_IN1, false }
	});

	vector<int> ctrlpins({
		CONTROLLINO_D0,
		CONTROLLINO_D3,
	});

	vector<int> forcepins({
		CONTROLLINO_D1,
		CONTROLLINO_D4,
	});

	advanceTimeAbit(100);
	// start in auto
	for (it=ctrlpins.begin(); it!=ctrlpins.end(); it++) {
		TEST_ASSERT(digitalRead(*it));
	}

	for (it=forcepins.begin(); it!=forcepins.end(); it++) {
		TEST_ASSERT(!digitalRead(*it));
	}

	// push the button
	for (init=inpins.begin(); init!=inpins.end(); init++) {
		digitalWrite(init->pinid,1);
	}

	advanceTimeAbit(100);
	for (init=inpins.begin(); init!=inpins.end(); init++) {
		if (!init->longpush) digitalWrite(init->pinid,0);
	}

	advanceTimeAbit(1500);
	for (init=inpins.begin(); init!=inpins.end(); init++) {
		if (init->longpush) digitalWrite(init->pinid,0);
	}
	advanceTimeAbit(100);
	// we now should be in forced
	for (it=forcepins.begin(); it!=forcepins.end(); it++) {
		TEST_ASSERT(digitalRead(*it));
	}
}

void ControllerTest::testDimmers() {
	vector<int> inpins({
			CONTROLLINO_A0,
			CONTROLLINO_A1,
	});
	vector<int> outpins({
		CONTROLLINO_D10,
		CONTROLLINO_D11,
	});

	vector<int>::iterator it;

	// hold the button down
	for (it=inpins.begin(); it!=inpins.end(); it++) {
		digitalWrite(*it,1);
	}

	// long time to wait, because the dimmers go through the sync sequence
	advanceTimeAbit(20000);

	// outputs should be high
	for (it=outpins.begin(); it!=outpins.end(); it++) {
		TEST_ASSERT(digitalRead(*it));
	}

	advanceTimeAbit(100);
	for (it=inpins.begin(); it!=inpins.end(); it++) {
		digitalWrite(*it,0);
	}

	advanceTimeAbit(100);
	// outputs should be low again
	for (it=outpins.begin(); it!=outpins.end(); it++) {
		TEST_ASSERT(!digitalRead(*it));
	}
}

void ControllerTest::testGlobalButton() {
	int pin=CONTROLLINO_A9;
	int dimdom=CONTROLLINO_A0;
	int dimdomout=CONTROLLINO_D10;
	vector<int>::iterator it;
	vector<int> lightpins({
		CONTROLLINO_R4,
		CONTROLLINO_R6,
		CONTROLLINO_R9,
	});

	// we need to go through the initial sync sequence of the dimmers
	digitalWrite(dimdom,1);
	advanceTimeAbit(100);
	digitalWrite(pin,0);
	advanceTimeAbit(20000);

	// trigger global control
	digitalWrite(pin,1);
	advanceTimeAbit(1501);
	digitalWrite(pin,0);
	advanceTimeAbit(50,1);

	// lights should be off
	for (it=lightpins.begin(); it!=lightpins.end(); it++) {
		TEST_ASSERT(!digitalRead(*it));
	}

	// the dimmer off sequence should have started
	// 100 ms off
	TEST_ASSERT(!digitalRead(dimdomout));
	// dimmer passthrough should not work
	digitalWrite(dimdom,1);
	advanceTimeAbit(1);
	if (digitalRead(dimdomout)) {
		digitalWrite(dimdom,0);
		TEST_ASSERT(!digitalRead(dimdomout));
	}
	advanceTimeAbit(20000);

	// dimmer passthrough should work again
	digitalWrite(dimdom,1);
	advanceTimeAbit(10);
	TEST_ASSERT(digitalRead(dimdomout));
	digitalWrite(dimdom,0);
	advanceTimeAbit(10);
	TEST_ASSERT(!digitalRead(dimdomout));
}
