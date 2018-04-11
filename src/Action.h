/*
 * Action.h
 *
 *  Created on: Feb 22, 2018
 *      Author: fraco
 */

#ifndef ACTION_H_
#define ACTION_H_

#include <list>

#include "sigslot.h"

class Action {
public:
	virtual void doit() = 0;
};

template<class Target>
class FunAction: public Action {
public:
	FunAction(Target * target, void (Target::*fun)()) :
			fun(fun), target(target) {
	}
	;
	void doit() {
		(target->*fun)();
	}
private:
	void (Target::*fun)();
	Target * target;
};

class ActionList: public Action, public sigslot::has_slots<> {

public:
	ActionList(){};
	void append(Action * act) {
		actions.push_back(act);
	}

	void doit() {
		std::list<Action *>::iterator it = actions.begin();
		for (; it != actions.end(); it++) {
			(*it)->doit();
		}
	}
private:
	std::list<Action *> actions;
};

#endif /* ACTION_H_ */
