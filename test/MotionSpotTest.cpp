/*
 * MotionSpotTest.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: fraco
 */

#include "MotionSpotTest.h"

#include <MotionSpot.h>
#include <Arduino.h>
#include <Controllino.h>
#include <cpptest-suite.h>
#include <cpptest-assert.h>


MotionSpotTest::~MotionSpotTest() {
	// TODO Auto-generated destructor stub
}

void MotionSpotTest::testBasic(){
	// starts in mode auto

	ms->handle();
	TEST_ASSERT(ctrlpin.read());
	TEST_ASSERT(!forcepin.read());

	// short push to mode forced
	shortsig.emit();
	(*time)++;
	ms->handle();

	TEST_ASSERT(forcepin.read());

	// short push to off
	shortsig.emit();
	(*time)++;
	ms->handle();
	TEST_ASSERT(!ctrlpin.read());
	TEST_ASSERT(!forcepin.read());

	// back to auto
	shortsig.emit();
	(*time)++;
	ms->handle();

	TEST_ASSERT(ctrlpin.read());
}

void MotionSpotTest::testLong(){

	// start in Auto
	ms->handle();
	TEST_ASSERT(ctrlpin.read());
	TEST_ASSERT(!forcepin.read());

	longsig.emit();
	(*time)++;
	ms->handle();

	// goes to forced

	TEST_ASSERT(forcepin.read());

	shortsig.emit();
	(*time)++;
	ms->handle();

	// goes to off
	TEST_ASSERT(!ctrlpin.read());
	TEST_ASSERT(!forcepin.read());

	// goes to forced
	longsig.emit();
	(*time)++;
	ms->handle();

	TEST_ASSERT(forcepin.read());

	longsig.emit();
	(*time)++;
	ms->handle();

	// off again
	TEST_ASSERT(!ctrlpin.read());
	TEST_ASSERT(!forcepin.read());
	ms->handle();
	(*time)+=100000;
	TEST_ASSERT(!ctrlpin.read());
	TEST_ASSERT(!forcepin.read());

}

void MotionSpotTest::setup(){
	TestWithTime::setup();

	actor = ms = new MotionSpot(ctrlpin, forcepin, indicatorpin);
	shortsig.connect(ms,&MotionSpot::shortpressed);
	longsig.connect(ms,&MotionSpot::longpressed);

	pinReset();

	ms->setup();
	ms->handle();

}

void MotionSpotTest::tear_down(){
	TestWithTime::tear_down();

	delete ms;
	actor = ms = NULL;
}
