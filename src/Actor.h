/*
 * Actor.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#ifndef ACTOR_H_
#define ACTOR_H_

class Actor {
public:
	virtual void handle()=0;
	virtual void setup()=0;
};

#endif /* ACTOR_H_ */
