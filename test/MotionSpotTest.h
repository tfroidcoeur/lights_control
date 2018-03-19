/*
 * MotionSpotTest.h
 *
 *  Created on: Mar 17, 2018
 *      Author: fraco
 */

#ifndef MOTIONSPOTTEST_H_
#define MOTIONSPOTTEST_H_

#include <cpptest-suite.h>
#include <sigslot.h>

#include "TestWithTime.h"
#include "Controllino.h"

class MotionSpot;

class MotionSpotTest: public Test::Suite, public sigslot::has_slots<>, public TestWithTime {
public:
	MotionSpotTest(): ctrlpin( CONTROLLINO_D0), forcepin(CONTROLLINO_D1), indicatorpin(CONTROLLINO_D2) {
		TEST_ADD(MotionSpotTest::testBasic)
		TEST_ADD(MotionSpotTest::testLong)
	}
	virtual ~MotionSpotTest();
private:
	void testBasic();
	void testLong();
	MotionSpot * ms;
	sigslot::signal0<> shortsig;
	sigslot::signal0<> longsig;
	int ctrlpin, forcepin, indicatorpin;
protected:
	void setup();
	void tear_down();
};

#endif /* MOTIONSPOTTEST_H_ */
