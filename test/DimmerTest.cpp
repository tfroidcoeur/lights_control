#include "DimmerTest.h"
#include "Controllino.h"

#include <Dimmer.h>
#include <cpptest-suite.h>
#include <cpptest-assert.h>
#include <Time.h>
#include <sigslot.h>

DimmerTest::DimmerTest() : out(CONTROLLINO_D0) {
	TEST_ADD(DimmerTest::testOn)
	TEST_ADD(DimmerTest::testOff)
	TEST_ADD(DimmerTest::testDim)
}

DimmerTest::~DimmerTest() {
}

int DimmerTest::PulseDurationMs = 600;

void DimmerTest::setup() {
	actor=testdimmer = new Dimmer(&in, &out, "testdimmer", NULL, 0.2, 900, 400);
	TestWithTime::setup();
	testdimmer->setup();;
}

void DimmerTest::tear_down() {
	delete actor;
	actor=testdimmer=NULL;
	TestWithTime::tear_down();
}

void DimmerTest::sendPulse(int ms){
	advanceTimeAbit();
	in.set(1);
	advanceTimeAbit(ms);
	in.set(0);
	advanceTimeAbit();
}

void DimmerTest::testOn() {
	advanceTimeAbit();
	TEST_ASSERT(!testdimmer->isOn());
	sendPulse();
	TEST_ASSERT(testdimmer->isOn());
	advanceTimeAbit();
}

void DimmerTest::testOff() {
	advanceTimeAbit();
	sendPulse();
	TEST_ASSERT(testdimmer->isOn());
	sendPulse();
	TEST_ASSERT(!testdimmer->isOn());
}

void DimmerTest::testDim() {
	advanceTimeAbit();
	// turn it on
	sendPulse(390);
	TEST_ASSERT(testdimmer->isOn());

	// dim up 1 second = 20%
	sendPulse(1900);
	TEST_ASSERT(testdimmer->isOn());
	TEST_ASSERT_DELTA(testdimmer->getLevel(), 0.2, 0.001);

	// dim down 2 second = 40%
	sendPulse(2900);
	TEST_ASSERT(testdimmer->isOn());
	TEST_ASSERT_DELTA(testdimmer->getLevel(), 0.0, 0.001);

	// dim up 5 second = 100%
	sendPulse(5900);
	TEST_ASSERT(testdimmer->isOn());
	TEST_ASSERT_DELTA(testdimmer->getLevel(), 1.0, 0.001);

	// dim down 10 second = 200%
	sendPulse(10900);
	TEST_ASSERT(testdimmer->isOn());
	TEST_ASSERT_DELTA(testdimmer->getLevel(), 0.0, 0.001);

	// dim up 10 second = 200%
	sendPulse(10900);
	TEST_ASSERT(testdimmer->isOn());
	TEST_ASSERT_DELTA(testdimmer->getLevel(), 1.0, 0.001);

	// dim down 3 second = 60%
	sendPulse(3900);
	TEST_ASSERT(testdimmer->isOn());
	TEST_ASSERT_DELTA(testdimmer->getLevel(), 0.4, 0.001);

	// off
	sendPulse(800);
	TEST_ASSERT(!testdimmer->isOn());

	// on (restore dimlevel)
	sendPulse(390);
	TEST_ASSERT(testdimmer->isOn());
	TEST_ASSERT_DELTA(testdimmer->getLevel(), 0.4, 0.001);

	// dim up 20%
	sendPulse(1900);
	TEST_ASSERT(testdimmer->isOn());
	TEST_ASSERT_DELTA(testdimmer->getLevel(), 0.6, 0.001);
}