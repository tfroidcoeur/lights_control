#ifndef STAIRCASETEST_H_
#define STAIRCASETEST_H_

#include <cpptest-suite.h>
#include <OutPin.h>
#include <sigslot.h>
#include "TestWithTime.h"
#include "Controllino.h"

class Staircase;

class StaircaseTest : public Test::Suite,
                      public sigslot::has_slots<>,
                      public TestWithTime {
public:

  StaircaseTest()  : outpin(CONTROLLINO_D0) {
    TEST_ADD(StaircaseTest::testBasic)
    TEST_ADD(StaircaseTest::testLong)
  }

  virtual ~StaircaseTest();

private:

  void testBasic();
  void testLong();
  Staircase *sc;
  sigslot::signal0<>shortsig;
  sigslot::signal0<>longsig;
  OutPin outpin;

protected:

  void setup();
  void tear_down();
};

#endif /* STAIRCASETEST_H_ */
