/*
 * SimpleButton.h
 *
 *  Created on: Mar 25, 2018
 *      Author: fraco
 */

#ifndef SIMPLEBUTTON_H_
#define SIMPLEBUTTON_H_

#include "Button.h"
#include "sigslot.h"
#include "InPin.h"

class SimpleButton : public Button {
public:

  SimpleButton(int shortdelay                      = 500, int longdelay= 1500) :
    Button() {
    longpress = NULL;
    addMode(ButtonMode(shortdelay, "short", &shortpress));
    addMode(ButtonMode(longdelay, "long", longpress));
  }

  virtual ~SimpleButton() {
    shortpress.disconnect_all();

    if (longpress) longpress->disconnect_all();
  }

  sigslot::signal0<>& getShortSignal() {
    return shortpress;
  }

  sigslot::signal0<>& getLongSignal() {
    if (!longpress) {
      longpress        = new sigslot::signal0<>();
      modes[1].pressed = longpress;
    }
    return *longpress;
  }

private:

  sigslot::signal0<>shortpress;
  sigslot::signal0<> *longpress;
};

#endif /* SIMPLEBUTTON_H_ */
