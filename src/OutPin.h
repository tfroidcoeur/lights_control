/*
 * OutPin.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */

#ifndef OUTPIN_H_
#define OUTPIN_H_

#include "Input.h"
#include "Actor.h"

class OutPin : public Actor, public Input {
public:

  OutPin(int id);
  virtual ~OutPin();

  /* toggle the output to the opposite state */
  void toggle();

  void write(int val);

  /* InPin */
  bool read();

  /* Actor */
  void setup();
  void handle() {}

private:

  int id;

  /* DigitalRead is not always correct, so
   * we track the last recorded output value as
   * the current expected state. */
  bool state;
};
#endif /* OUTPIN_H_ */
