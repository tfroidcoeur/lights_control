#ifndef DEBOUNCEDINPUT_H_
#define DEBOUNCEDINPUT_H_

#include "Actor.h"
#include "sigslot.h"
#include <stdint.h>
#include "Input.h"

struct Debounce {
	unsigned long changetime;
	int readval;
	int stableval;
};

class DebouncedInput : public NotifiedInput {
private:
	uint32_t debouncetime;
	Input * in;
	struct Debounce d;
public:
	DebouncedInput(Input * in, uint32_t debouncetime=20);
	Input * getRawInput() { return in;}
	virtual ~DebouncedInput(){
		delete  in;
	};
	/* Input */
	virtual int read();
	/* NotifiedInput */
	virtual sigslot::signal1<int> & getChangeSignal() {return changed;}

	/* Actor */
	virtual void handle();
	virtual void setup(){};

private:
	int debounce();
	sigslot::signal1<int> changed;
};

#endif /* DEBOUNCEDINPUT_H_ */
