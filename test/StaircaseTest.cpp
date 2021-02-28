#include "StaircaseTest.h"

#include <Staircase.h>
#include <Arduino.h>
#include <Controllino.h>
#include <cpptest-suite.h>
#include <cpptest-assert.h>


StaircaseTest::~StaircaseTest() {}

void StaircaseTest::testBasic() {
  sc->handle();
  TEST_ASSERT(!outpin.read());

  // short push to normal on
  shortsig.emit();
  advanceTimeAbit(1000);

  TEST_ASSERT(outpin.read());
  advanceTimeAbit(5 * 60 * 1000 + 3 * 10500, 10000);
  TEST_ASSERT(!outpin.read());
}

void StaircaseTest::testLong() {
  sc->handle();
  TEST_ASSERT(!outpin.read());

  // long push to normal on
  longsig.emit();
  advanceTimeAbit(1000);

  TEST_ASSERT(outpin.read());
  advanceTimeAbit(5 * 60 * 1000 + 3 * 10500, 10000);
  TEST_ASSERT(outpin.read());
  advanceTimeAbit(55 * 60 * 1000);
  TEST_ASSERT(!outpin.read());
}

void StaircaseTest::setup() {
  TestWithTime::setup();

  actor = sc = new Staircase(outpin, "teststairs");
  shortsig.connect(sc, &Staircase::shortpressed);
  longsig.connect(sc, &Staircase::longpressed);

  pinReset();

  sc->setup();
  sc->handle();
}

void StaircaseTest::tear_down() {
  TestWithTime::tear_down();

  delete sc;
  actor = sc = NULL;
}
