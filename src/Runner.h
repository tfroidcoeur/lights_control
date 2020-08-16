/*
 * Runner.h
 *
 *  Created on: Mar 12, 2018
 *      Author: fraco
 */

#ifndef RUNNER_H_
#define RUNNER_H_

#include <list>

#include "Actor.h"
#include <assert.h>

class Runner : public Actor {
public:
	Runner();
	virtual ~Runner();

	void addActor(Actor * actor) { 
		assert(actor);
		if (actor)
			actors.push_back(actor);
	};
	void removeActor(Actor * actor) { actors.remove(actor);};

	void handle(){
		for (std::list<Actor *>::iterator it = actors.begin(); it!=actors.end(); it++){
			(*it)->handle();
		}
	}
	void setup(){
		for (std::list<Actor *>::iterator it = actors.begin(); it!=actors.end(); it++){
			(*it)->setup();
		}
	}

private:
	std::list<Actor *> actors;

};

#endif /* RUNNER_H_ */
