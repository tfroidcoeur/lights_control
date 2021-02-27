/*
 * Switchable.h
 *
 *  Created on: Apr 2, 2018
 *      Author: fraco
 */

#ifndef SWITCHABLE_H_
#define SWITCHABLE_H_

class Switchable {
public:

  Switchable() {}

  virtual ~Switchable() {}

  virtual void on()   = 0;
  virtual void off()  = 0;
  virtual bool isOn() = 0;
  virtual void toggle() {
    if (isOn()) off(); else on();
  }
};

#endif /* SWITCHABLE_H_ */
