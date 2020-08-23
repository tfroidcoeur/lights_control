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
}

DimmerTest::~DimmerTest() {
}

int DimmerTest::PulseDurationMs = 600;

void DimmerTest::setup() {
	actor=testdimmer = new Dimmer(&in, &out, "testdimmer", NULL);
	TestWithTime::setup();
	testdimmer->setup();
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