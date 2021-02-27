/*
 * Time.cpp
 *
 *  Created on: Jan 5, 2018
 *      Author: fraco
 */

#include "Time.h"

Time::Time() : ms(0) {}

Time::~Time() {}

static Time *theTime = new Time();

unsigned long millis(void) {
  /* the Time can be NULL during initialization if called from for example InPin
   * returning 0 is reasonable then*/
  return theTime ? theTime->millis() : 0;
}

void setTheTime(Time *t) {
  theTime = t;
}

Time* getTheTime() {
  return theTime;
}

unsigned long Time::millis() {
  return ms;
}

Time Time::operator++(int) {
  ms++;
  return *this;
}

Time& Time::operator+=(int rhs) {
  ms += rhs;
  return *this;
}
