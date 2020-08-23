#ifndef DIMMERTEST_H_
#define DIMMERTEST_H_

#include <Dimmer.h>
#include <cpptest-suite.h>
#include <sigslot.h>
#include <iostream>

#include "TestWithTime.h"
#include "TestInput.h"

class Time;

class Dimmer;

class DimmerTest : public Test::Suite, public sigslot::has_slots<>, public TestWithTime {
public:
	DimmerTest();
	virtual ~DimmerTest();
	static int PulseDurationMs;
protected:
	void setup();
	void tear_down();
private:
	TestInput in;
	OutPin out;
	Dimmer * testdimmer;
	void testOn();
	void testOff();
	void sendPulse(int ms=600);

};

#endif /* DIMMERTEST_H_ */
