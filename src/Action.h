/*
 * Action.h
 *
 *  Created on: Feb 22, 2018
 *      Author: fraco
 */

#ifndef ACTION_H_
#define ACTION_H_

#include <Arduino.h>
#include <ArduinoSTL.h>
#include "sigslot.h"
#include "logging.h"
using namespace std;

class Action {
public:

  virtual ~Action() {}

  virtual void doit() = 0;
};

template<class Target>
class FunAction : public Action {
public:

  virtual ~FunAction() {}

  FunAction(Target *target, void(Target::*fun)()) :
    fun(fun), target(target) {}

  void doit() {
    (target->*fun)();
  }

private:

  void (Target::*fun)();
  Target *target;
};

class ActionList : public Action, public sigslot::has_slots<>{
public:

  ActionList() {}

  ~ActionList() {
    std::vector<Action *>::iterator it = actions.begin();

    for (; it != actions.end(); it++) {
      delete *it;
    }
  }

  void append(Action *act) {
    actions.push_back(act);
  }

  void doit() {
    std::vector<Action *>::iterator it = actions.begin();

    for (; it != actions.end(); it++) {
      (*it)->doit();
    }
  }

private:

  std::vector<Action *> actions;
};

#endif /* ACTION_H_ */
