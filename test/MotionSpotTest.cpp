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
	TEST_ASSERT(digitalRead(ctrlpin));
	TEST_ASSERT(!digitalRead(forcepin));

	// short push to mode forced
	shortsig.emit();
	(*time)++;
	ms->handle();

	TEST_ASSERT(digitalRead(forcepin));

	// short push to off
	shortsig.emit();
	(*time)++;
	ms->handle();
	TEST_ASSERT(!digitalRead(ctrlpin));
	TEST_ASSERT(!digitalRead(forcepin));

	// back to auto
	shortsig.emit();
	(*time)++;
	ms->handle();

	TEST_ASSERT(digitalRead(ctrlpin));
}

void MotionSpotTest::testLong(){

	// start in Auto
	ms->handle();
	TEST_ASSERT(digitalRead(ctrlpin));
	TEST_ASSERT(!digitalRead(forcepin));

	longsig.emit();
	(*time)++;
	ms->handle();

	// goes to forced

	TEST_ASSERT(digitalRead(forcepin));

	shortsig.emit();
	(*time)++;
	ms->handle();

	// goes to off
	TEST_ASSERT(!digitalRead(ctrlpin));
	TEST_ASSERT(!digitalRead(forcepin));

	// goes to forced
	longsig.emit();
	(*time)++;
	ms->handle();

	TEST_ASSERT(digitalRead(forcepin));

	longsig.emit();
	(*time)++;
	ms->handle();

	// off again
	TEST_ASSERT(!digitalRead(ctrlpin));
	TEST_ASSERT(!digitalRead(forcepin));
	ms->handle();
	(*time)+=100000;
	TEST_ASSERT(!digitalRead(ctrlpin));
	TEST_ASSERT(!digitalRead(forcepin));

}

void MotionSpotTest::setup(){
	TestWithTime::setup();

	ms = new MotionSpot(ctrlpin, forcepin, indicatorpin);
	shortsig.connect(ms,&MotionSpot::shortpressed);
	longsig.connect(ms,&MotionSpot::longpressed);

	pinReset();

	ms->setup();
	ms->handle();

}

void MotionSpotTest::tear_down(){
	TestWithTime::tear_down();

	delete ms;
	ms = NULL;
}
