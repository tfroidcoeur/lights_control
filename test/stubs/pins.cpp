/*
 * pins.cpp
 *
 *  Created on: Dec 31, 2017
 *      Author: fraco
 */

#include <stdint.h>
#include <string.h>

#define MAX_PINS 128

static int pins[MAX_PINS] = {};

void pinReset() {
  memset(pins, 0, sizeof(int) * MAX_PINS);
}

void pinMode(uint8_t id, uint8_t mode) {}

void digitalWrite(uint8_t id, uint8_t value) {
  if (id >= MAX_PINS) return;  // FIXME trigger an assert

  pins[id] = value;
}

int digitalRead(uint8_t id) {
  if (id >= MAX_PINS) return 0;  // FIXME trigger an assert

  return pins[id];
}
