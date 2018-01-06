/*
 * Time.h
 *
 *  Created on: Jan 5, 2018
 *      Author: fraco
 */

#ifndef STUBS_TIME_H_
#define STUBS_TIME_H_

class Time {
public:
	Time();
	virtual ~Time();
	unsigned long millis();
	Time operator++(int);     // Postfix increment operator.
	Time& operator+=(int rhs);
private:
	unsigned long ms;
};
Time * getTheTime();
void setTheTime(Time * t);

#endif /* STUBS_TIME_H_ */
