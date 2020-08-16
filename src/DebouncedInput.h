#ifndef DEBOUNCEDINPUT_H_
#define DEBOUNCEDINPUT_H_

#include "Actor.h"
#include "sigslot.h"
#include <stdint.h>
#include "Input.h"

class DebouncedInput : public NotifiedInput {
private:
	uint32_t debouncetime;
	Input * in;
	unsigned long changetime;
	int readval;
	int stableval;
public:
	DebouncedInput(Input * in, bool owninput = false, uint32_t debouncetime=20);
	Input * getRawInput() { return in;}
	virtual ~DebouncedInput(){
		if (owninput) delete in;
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
	bool owninput;
};

#endif /* DEBOUNCEDINPUT_H_ */
