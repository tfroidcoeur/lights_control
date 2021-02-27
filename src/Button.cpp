/*
 * Button.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "Button.h"

std::ostream & operator<<(std::ostream& os, const ButtonMode& m) {
#ifdef DEBUG_MODE_NAME
  const char *name = m.name ? m.name : "NULL"
#else // ifdef DEBUG_MODE_NAME
  const char *name = "unknown";
#endif // ifdef DEBUG_MODE_NAME
                     return os << "mode " << name << "(" << m.delay << ","
                               << static_cast<void *>(m.pressed) << ")";
}

void Button::emit(const ButtonMode& mode) const {
  COUT_DEBUG(
    cout << "button " << hex << this << dec << " notify: " << mode.name << endl);

  if (mode.pressed) mode.pressed->emit();
}

void Button::handle() {
  if (!pending || (curmode == modes.end())) {
    return;
  }

  // handle pin, could call callbacks
  COUT_DEBUG(
    cout << "button handler " << (pending ? "" : "not ") << "pending ms: " << millis() << " started " << started <<
    endl); COUT_DEBUG(cout << "curmode: " << *curmode << endl);

  while (pending && (millis() - started > curmode->delay)) {
    const ButtonMode& prevmode = *curmode;

    if (++curmode == modes.end()) {
      pending = false;
      emit(prevmode);
      break;
    }
    COUT_DEBUG(cout << "next " << *curmode << endl);
  }
}

void Button::pinChanged(int value) {
  COUT_DEBUG(Serial.print("button notify ")); COUT_DEBUG(Serial.println(value));

  if (value) {
    // went high
    started = millis();
    pending = true;
    curmode = modes.begin();
    COUT_DEBUG(Serial.println("button started pending"));
  } else if (pending) {
    COUT_DEBUG(cout << "stop pending " << *curmode << endl);
    pending = false;

    if (curmode != modes.end()) emit(*curmode);
    curmode = modes.begin();
  }
}
