/*
 * ControllerTest.h
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */

#ifndef CONTROLLERTEST_H_
#define CONTROLLERTEST_H_

#include <cpptest-suite.h>
#include "TestWithTime.h"

class ControllerTest : public Test::Suite, public TestWithTime {
public:

  ControllerTest();
  virtual ~ControllerTest();

  virtual void setup();
  virtual void tear_down();

private:

  void testTeleruptors();
  void testMotionSpot();
  void testDimmers();
  void testGlobalButton();
};

#endif /* CONTROLLERTEST_H_ */
