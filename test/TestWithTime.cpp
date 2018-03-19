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
