/*
 * Actor.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#ifndef ACTOR_H_
#define ACTOR_H_

/**
 * An actor is an object that implements behaviour.
 */
class Actor {
public:

  virtual ~Actor() {}

  /**
   * method called regularly in the loop
   */
  virtual void handle() = 0;

  /**
   * setup function
   */
  virtual void setup() = 0;
};

#endif /* ACTOR_H_ */
