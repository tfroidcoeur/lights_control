/*
 * SequencerTest.cpp
 *
 *  Created on: Mar 31, 2018
 *      Author: fraco
 */
#define DEBUG

#include <cpptest-assert.h>
#include "SequencerTest.h"
#include <Sequencer.h>
#include "Controllino.h"

SequencerTest::SequencerTest(): p(CONTROLLINO_D0) {
	TEST_ADD(SequencerTest::testSequence)
}

SequencerTest::~SequencerTest() {
	// TODO Auto-generated destructor stub
}

void SequencerTest::setup(){
	TestWithTime::setup();
	actor = seq = new Sequencer(p);
	seq->setup();
}

void SequencerTest::tear_down(){
	TestWithTime::tear_down();
	delete seq;
	actor = seq = NULL;
}

void SequencerTest::testSequence() {
	SeqPattern * pat = Sequencer::createPattern("100*1 1000*0 10*1");
	//initial value
	p.write(111);

	seq->start(pat);

	// 10 - 90  ms
	for (int i = 1; i< 10; i++){
		advanceTimeAbit(10,4);
		TEST_ASSERT(p.read());
	}
	// 101 ms
	advanceTimeAbit(11,10);
	TEST_ASSERT(!p.read());

	// 101 - 1001 ms
	for (int i = 1; i< 10; i++){
		advanceTimeAbit(100,200);
		TEST_ASSERT(!p.read());
	}
	// 1101 ms
	advanceTimeAbit(100,100);
	TEST_ASSERT(p.read());

	// 1109
	advanceTimeAbit(8,2);
	TEST_ASSERT(p.read());

	// 1111
	advanceTimeAbit(2,1);

	TEST_ASSERT(p.read() == 111);
	delete pat;
}

