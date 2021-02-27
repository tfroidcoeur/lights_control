#ifndef DEBOUNCEDINPUT_H_
#define DEBOUNCEDINPUT_H_

#include "Arduino.h"
#include "Actor.h"
#include "sigslot.h"
#include <stdint.h>
#include "Input.h"

class DebouncedInput : public NotifiedInput {
public:

  DebouncedInput(Input   *in,
                 bool     owninput = false,
                 uint32_t debouncetime = 20,
                 uint16_t repeatTimeHighMs = 0);
  Input* getRawInput() {
    return in;
  }

  virtual ~DebouncedInput() {
    if (owninput) delete in;
  }

  /* Input */
  virtual bool                   read();

  /* NotifiedInput */
  virtual sigslot::signal1<int>& getChangeSignal() {
    return changed;
  }

  /* Actor */
  virtual void handle();
  virtual void setup() {}

private:

  int debounce();

  Input *in;
  sigslot::signal1<int> changed;
  unsigned long changetime;
  unsigned long lastreport;
  uint16_t repeatTimeHighMs;
  uint8_t  debouncetime;
  bool     readval;
  bool     stableval;
  bool     owninput;
};

#endif /* DEBOUNCEDINPUT_H_ */
