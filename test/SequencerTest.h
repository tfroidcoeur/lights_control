/*
 * SequencerTest.h
 *
 *  Created on: Mar 31, 2018
 *      Author: fraco
 */

#ifndef SEQUENCERTEST_H_
#define SEQUENCERTEST_H_

#include <cpptest-suite.h>
#include <sigslot.h>

#include "TestWithTime.h"
#include "Sequencer.h"
#include "OutPin.h"
#include "Controllino.h"


class SequencerTest: public Test::Suite,
		public sigslot::has_slots<>,
		public TestWithTime{
public:
	SequencerTest();
	virtual ~SequencerTest();
	void testSequence();
protected:
	void setup();
	void tear_down();
private:
	OutPin p;
	Sequencer * seq;
};

#endif /* SEQUENCERTEST_H_ */
