/*
 * ButtonTest.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: fraco
 */

#include "ButtonTest.h"

#include <Button.h>
#include <cpptest-suite.h>
#include <sigslot.h>
#include <Time.h>

ButtonTest::ButtonTest()  {
	testbutton = new Button();
	TEST_ADD(ButtonTest::pressTest)
}

ButtonTest::~ButtonTest() {
	// TODO Auto-generated destructor stub
}

void ButtonTest::setup() {
	ButtonMode m;
	m.delay = 100;
	m.pressed = &press1;
	testbutton->addMode(m);
	m.delay = 200;
	m.pressed = &press2;
	testbutton->addMode(m);
	press1.connect(this, &ButtonTest::notifyPress1);
	press2.connect(this, &ButtonTest::notifyPress2);
	testbutton->setup();
}

void ButtonTest::tear_down() {
	delete testbutton;
}

void ButtonTest::pressTest() {
	Time t;
	Time * orig = getTheTime();
	setTheTime(&t);

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
	t+=20;
	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==0);
	testbutton->handle();
	TEST_ASSERT(press1count==0);
	TEST_ASSERT(press2count==0);
	t+=1;
	testbutton->pinChanged(0);
	testbutton->handle();
	TEST_ASSERT(press1count==1);
	TEST_ASSERT(press2count==0);
}

