/*
 * Teleruptor.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */
#ifndef TELERUPTOR_H_
#define TELERUPTOR_H_

#include "InPin.h"
#include "OutPin.h"
#include "sigslot.h"
#include "Stashable.h"
#include "Switchable.h"
#include "MqttNode.h"

class Teleruptor: public Actor,
		public sigslot::has_slots<>,
		public Switchable,
		public Stashable,
		public MqttNode {
public:
	Teleruptor(NotifiedInput * inpin, OutPin * outpin, const char* name, MqttNode * parent = NULL);
	Teleruptor(sigslot::signal0<> & sig, OutPin * outpin, const char* name, MqttNode * parent = NULL);
	virtual ~Teleruptor();
	virtual void handle(void);
	virtual void setup(void);
	void notifyInPin(int value);
	void pressed();

	// mqtt node
	virtual void update(string const& path, string const & value);
	virtual void refresh();
// Switchable
	virtual void on();
	virtual void off();
	virtual bool isOn();
	virtual void toggle();
// Stashable
	virtual void save();
	virtual void restore();
private:
	OutPin &out;
	bool savedstate;
	static const string ON;
	static const string OFF;
};

#endif /* TELERUPTOR_H_ */
