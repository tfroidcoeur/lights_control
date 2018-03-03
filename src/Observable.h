/*
 * Observable.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include "List.h"
#include "Observer.h"

class Observable {
public:
	Observable(){};
	virtual ~Observable(){};
	virtual void setObserver(Observer * o) { addObserver(o); }
	virtual void addObserver(Observer * o) { obs.add(o); }
	virtual void removeObserver(Observer * o) { obs.remove(o); }
protected:
	void notify(void) {
		for (List<Observer>::iterator it = obs.begin(); it!=obs.end(); it++){
			(*it).notify(this);
		};
	}
private:
	List<Observer> obs;
};

#endif /* OBSERVABLE_H_ */
