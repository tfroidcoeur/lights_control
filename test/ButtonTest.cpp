/*
 * ButtonTest.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: fraco
 */

#include "ButtonTest.h"

#include <Button.h>
#include <cpptest-suite.h>
#include <cpptest-assert.h>
#include <Time.h>
#include <sigslot.h>

ButtonTest::ButtonTest()  {
	testbutton = new Button();
	TEST_ADD(ButtonTest::pressLongTest)
	TEST_ADD(ButtonTest::pressShortTest)
	TEST_ADD(ButtonTest::pressTimeoutTest)
}

ButtonTest::~ButtonTest() {
	delete testbutton;
	testbutton=NULL;
}

void ButtonTest::setup() {
	TestWithTime::setup();

	ButtonMode m1(100, "short", &press1);
	ButtonMode m2(200, "long", &press2);
	testbutton->addMode(m1);
	testbutton->addMode(m2);
	press1.connect(this, &ButtonTest::notifyPress1);
	press2.connect(this, &ButtonTest::notifyPress2);
	testbutton->setup();
	press1count=0;
	press2count=0;
}

void ButtonTest::tear_down() {
	TestWithTime::tear_down();
	press1.disconnect(this);
	press2.disconnect(this);
}

void ButtonTest::pressLongTest() {
	Time &t=*time;

	testbutton->handle();
	t+=100;

	// notify pin high
	testbutton->pinChanged(1);
	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==0);
	testbutton->handle();

	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==0);

	// elapse a little bit of time
	for (;t<300; t+=10){
		testbutton->handle();
		TEST_ASSERT(press1count==0);
		TEST_ASSERT(press2count==0);
	}
	t++;
	testbutton->pinChanged(0);
	testbutton->handle();
	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==1);
}

void ButtonTest::pressTimeoutTest() {
	Time &t=*time;

	testbutton->handle();
	t+=100;

	// notify pin high
	testbutton->pinChanged(1);
	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==0);
	testbutton->handle();

	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==0);

	// elapse a little bit of time
	for (;t<200; t+=10){
		testbutton->handle();
		TEST_ASSERT(press1count==0);
		TEST_ASSERT(press2count==0);
	}
	t+=201;
	testbutton->handle();
	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==1);

	// bring the pin down now
	t+=1;
	testbutton->pinChanged(0);
	testbutton->handle();
}

void ButtonTest::pressShortTest() {
	Time &t=*time;

	testbutton->handle();
	t+=100;

	// notify pin high
	testbutton->pinChanged(1);
	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==0);
	testbutton->handle();

	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==0);

	// elapse a little bit of time
	for (;t<200; t+=10){
		testbutton->handle();
		TEST_ASSERT(press1count==0);
		TEST_ASSERT(press2count==0);
	}
	t++;
	testbutton->pinChanged(0);
	testbutton->handle();
	TEST_ASSERT(press1count==1);
	TEST_ASSERT(press2count==0);
}

