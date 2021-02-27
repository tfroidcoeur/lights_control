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

#include "TestWithTime.h"

class Time;

class Button;

class ButtonTest : public Test::Suite, public sigslot::has_slots<>,
                   public TestWithTime {
public:

  ButtonTest();
  virtual ~ButtonTest();

protected:

  void setup();
  void tear_down();

private:

  Button *testbutton;
  sigslot::signal0<>press1;
  int press1count = 0;
  void notifyPress1() {
    press1count++; /* cout << "pressed 1 (" << press1count <<")" << endl; */
  }

  sigslot::signal0<>press2;
  void notifyPress2() {
    press2count++; /* cout << "pressed 2 (" << press2count <<")" << endl; */
  }

  int press2count = 0;
  void pressShortTest();
  void pressShortTestMany();
  void pressTimeoutTest();
  void pressLongTest();
};

#endif /* BUTTONTEST_H_ */
