/*
 * inpin.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */


#include "Arduino.h"
#include "ArduinoSTL.h"
#include "DebouncedInput.h"

// #define DEBUG
#undef DEBUG
#include "logging.h"
using namespace std;

bool DebouncedInput::read() {
  return stableval;
}

int DebouncedInput::debounce() {
  unsigned long now = millis();
  bool val          = in->read();
  bool previousval  = readval;

  // transfer the value
  readval = val;

  unsigned long elapsed = now - changetime;
  COUT_DEBUG(cout << "pin: " << in << " elapsed " << elapsed << endl);

  if (previousval != val) {
    // if changed from last read, reset the d timer
    changetime = now;
    COUT_DEBUG(
      cout << "debouncing input pin:" << in << " val " << val << " dt: " << debouncetime <<
      endl);
  } else if ((val != stableval) && (elapsed > debouncetime)) {
    // new stable value
    COUT_DEBUG(cout << "dd input pin:" << in << " to value " << val << endl);
    stableval = val;
    return true;
  }
  return false;
}

DebouncedInput::DebouncedInput(Input   *in,
                               bool     owninput,
                               uint32_t debouncetime,
                               uint16_t repeatTimeHighMs) :
  in(in), repeatTimeHighMs(repeatTimeHighMs), debouncetime(debouncetime),
  owninput(owninput) {
  // if someone is pressing a button during boot, it
  // will be dd and then reported as a buttonpush
  readval    = stableval = 0;
  lastreport = changetime = millis();
}

void DebouncedInput::handle() {
  if (debounce()) {
    // state changed after debouncing
    // call state change handler
    COUT_DEBUG(
      cout << "dd input pin:" << in << " emit " << hex << &changed << dec << " val: " << stableval <<
      endl);
    lastreport = millis();
    changed.emit(stableval);
  } else if (stableval && repeatTimeHighMs &&
             (millis() - lastreport > repeatTimeHighMs)) {
    // report stable high every repeatTimeHighMs
    lastreport = millis();
    changed.emit(stableval);
  }
}
