/*
 * Event.h
 *
 *  Created on: Feb 22, 2018
 *      Author: fraco
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "Observable.h"

class Event : public Observable{
public:
	Event(){};
	virtual ~Event(){};
protected:
	void trigger(){ notify(); }
};

#endif /* EVENT_H_ */
