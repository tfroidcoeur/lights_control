/*
 * Time.cpp
 *
 *  Created on: Jan 5, 2018
 *      Author: fraco
 */

#include "Time.h"

Time::Time() : ms(0) {
}

Time::~Time() { }

static Time * theTime = new Time();

unsigned long millis(void) {
	return theTime->millis();
}

void setTheTime(Time * t){
	theTime  = t;
}

Time * getTheTime(){
	return theTime;
}

unsigned long Time::millis() {
	return ms;
}

Time Time::operator++(int){
	ms++;
	return *this;
}

Time& Time::operator+=(int rhs){
	ms+=rhs;
	return *this;
}

