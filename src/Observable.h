/*
 * Observable.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_
#include "Observer.h";

class Observable {
public:
	Observable();
	virtual ~Observable();
	virtual void setObserver(Observer * o) {
		observer = o;
	}
protected:
	void notify(void) {
		if (observer) {
			observer->notify(this);
		};
	}
private:
	Observer * observer;
};

#endif /* OBSERVABLE_H_ */
