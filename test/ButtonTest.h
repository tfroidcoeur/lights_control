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

class ButtonTest : public Test::Suite, public sigslot::has_slots<> {
public:
	ButtonTest();
	virtual ~ButtonTest();
protected:
	void setup();
	void tear_down();
private:
	Button * testbutton;
};

#endif /* BUTTONTEST_H_ */
