/*
 * ButtonTest.h
 *
 *  Created on: Mar 12, 2018
 *      Author: fraco
 */

#ifndef BUTTONTEST_H_
#define BUTTONTEST_H_

#include <Button.h>
#include <cpptest-suite.h>
#include <sigslot.h>
#include <iostream>

#include "stubs/Time.h"

class Button;

class ButtonTest : public Test::Suite, public sigslot::has_slots<> {
public:
	ButtonTest();
	virtual ~ButtonTest();
protected:
	void setup();
	void tear_down();
private:
	Button * testbutton;
	sigslot::signal0<> press1;
	int press1count=0;
	void notifyPress1(){press1count++; cout << "pressed 1 (" << press1count <<")" << endl;};
	sigslot::signal0<> press2;
	void notifyPress2(){press2count++; cout << "pressed 2 (" << press2count <<")" << endl;};
	int press2count=0;
	void pressShortTest();
	void pressLongTest();
	Time *time;
	Time * orig;

};

#endif /* BUTTONTEST_H_ */
