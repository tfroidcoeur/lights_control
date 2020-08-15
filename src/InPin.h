#ifndef INPIN_H_
#define INPIN_H_

#include "Actor.h"
#include "Input.h"

class InPin : public Actor, public Input {
private:
	int id;
public:
	InPin(int id);

	/* Input */
	virtual int read();

	/* Actor */
	virtual void handle();
	virtual void setup();
};



#endif /* INPIN_H_ */
