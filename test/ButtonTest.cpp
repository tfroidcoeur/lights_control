/*
 * ButtonTest.cpp
 *
 *  Created on: Mar 12, 2018
 *      Author: fraco
 */

#include "ButtonTest.h"


ButtonTest::ButtonTest() {
	TEST_ADD(ButtonTest::)

}

ButtonTest::~ButtonTest() {
	// TODO Auto-generated destructor stub
}

void ButtonTest::setup(){
	testbutton = new Button()
}

void ButtonTest::tear_down(){

}

class InPinTest : public Test::Suite, public sigslot::has_slots<>
{
protected:
	// just count number of notifications
	virtual void notify(int value){
		notified++;
	}

private:
	int notified=0;
	// Test debounce behaviour
	void debounceTest() {
		// use our own time, we can play with
		Time t;
		Time * orig=getTheTime();
		setTheTime(&t);

		// pin under test
		InPin p(CONTROLLINO_A1);
		p.changed.connect(this, &InPinTest::notify);
		p.setup();

		// run the pin a first time
		p.handle();

		// and again 500 ms later
		t+=500;
		p.handle();

		TEST_ASSERT(notified==0);


		// now activate the input pin
		digitalWrite(CONTROLLINO_A1,1);


		// the pin should not be notfied before it has been stable for 20 ms
		p.handle();
		t+=18;
		p.handle();
		TEST_ASSERT(notified==0);

		t++;
		p.handle();
		TEST_ASSERT(notified==0);

		// bring it down before it actually triggers
		// this whole episode should not be a trigger
		digitalWrite(CONTROLLINO_A1,0);
		p.handle();
		t++;
		p.handle();
		TEST_ASSERT(notified==0);

		// pin active again
		digitalWrite(CONTROLLINO_A1,1);
		p.handle();
		t+=18;
		p.handle();
		TEST_ASSERT(notified==0);
		t+=3;
		p.handle();
		// after 21 ms, finally, the pin should have notified
		TEST_ASSERT(notified==1);

		// and check if it notifies off also
		digitalWrite(CONTROLLINO_A1,0);
		p.handle();
		t+=22;
		p.handle();
		TEST_ASSERT(notified==2);


		setTheTime(orig);

	}
};
