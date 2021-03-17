/*
 * Sequencer.h
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include "Actor.h"
#include "OutPin.h"
#include "sigslot.h"
#include <string>
#include <stdint.h>

/**
 * individual seq element
 * duration: duration in ms
 * value: value to set on the pin
 */
class SeqElement {
public:

  SeqElement() {
    duration = 0;
  }

  SeqElement(const SeqElement& orig) {
    duration = orig.duration;
  }

  SeqElement(uint32_t duration, bool value) {
    this->duration = duration << 1 | value;
  }

  uint32_t getDuration() {
    return duration >> 1;
  }

  void setDuration(uint32_t d) {
    duration = (d << 1) | getValue();
  }

  bool getValue() {
    return duration & 0x1;
  }

  void setValue(bool v) {
    duration = (duration & ~0x1) | v;
  }

private:

  uint32_t duration;
};


/**
 * a seq pattern is a series of seq elements plus some info
 */
class SeqPattern {
public:

  int repeatcount;
  SeqElement *elements;
  SeqPattern(int repeatcount = 0, SeqElement *elements = NULL) :
    repeatcount(repeatcount), elements(elements) {}

  virtual ~SeqPattern() {
    if (elements) delete[] elements;
  }
};

/**
 * Blinking led makes an Outpin blink capable.
 * TODO: nicer to move functionality to BlinkPattern and provide an
 * Outpin to the start function - maybe
 */
class Sequencer :  public Actor {
public:

  Sequencer(OutPin& pin);
  virtual ~Sequencer();

  /* functions for Actor */
  virtual void setup() {}

  virtual void handle();

  /* API */
  void         start(SeqPattern *pattern);

  /* start multiple patterns, terminated by NULL pointer */
  void         startSeries(SeqPattern **pattern);
  SeqPattern * stop(bool restore);
  SeqPattern * stop() {
    return stop(true);
  }

  bool               isRunning();
  static SeqPattern* createPattern(std::string pat);
  sigslot::signal0<>done;

private:

  void activate();
  OutPin *out;
  int origvalue;
  SeqPattern *pattern;
  unsigned long startTime;
  int activeStep;
  int repeatcount;
  SeqPattern **pat_next;
  void endPattern(int value);
  void printStep(SeqElement& step);
  void _start(SeqPattern *pattern);
};

#endif /* SEQUENCER_H_ */
