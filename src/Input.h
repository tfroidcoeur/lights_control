#ifndef INPUT_H_
#define INPUT_H_
#include "sigslot.h"
#include "Actor.h"
class Input {
public:
	virtual bool read()=0;
	virtual ~Input(){}
};

class NotifiedInput: public Input, public Actor {
public:
	virtual bool read()=0;
	virtual sigslot::signal1<int> & getChangeSignal()=0;
	virtual ~NotifiedInput(){}
};
#endif /* INPUT_H_ */
