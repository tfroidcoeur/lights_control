/*
 * TestWithTime.cpp
 *
 *  Created on: Mar 17, 2018
 *      Author: fraco
 */

#include "TestWithTime.h"
#include <stddef.h>
#include <iostream>
#include <ArduinoSTL.h>
using namespace std;
#include "logging.h"

TestWithTime::TestWithTime(): orig(NULL) {
//	cout << "TestWithTime" << endl;
	time = new Time();
}

TestWithTime::~TestWithTime() {
//	cout << "~TestWithTime" << endl;
	if (time) delete time;
	time=NULL;
}


void TestWithTime::setup() {
//	cout << "config time" << endl;
	orig = getTheTime();
	setTheTime(time);
}

void TestWithTime::tear_down() {
//	cout << "restore orig time" << endl;
	setTheTime(orig);
}

void TestWithTime::advanceTimeAbit(int delay, int inc){
	int done=delay;
	actor->handle();
	while (done>0) {
		int step=inc>done?done:inc;
		*time+=step;
		done-=step;
		actor->handle();
	}
	COUT_DEBUG(cout << "adv time: +" << delay << " to " << time->millis() << endl);
}

