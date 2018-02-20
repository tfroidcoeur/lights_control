/*
 * Observer.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_
class Observable;

class Observer {
	friend class Observable;
public:
	virtual void notify(Observable * o)=0;
	virtual ~Observer();
};

#endif /* OBSERVER_H_ */
