/*
 * BlinkingLed.cpp
 *
 *  Created on: Dec 17, 2017
 *      Author: fraco
 */

#include "Sequencer.h"

#include <Arduino.h>
#include <ArduinoSTL.h>
#include <iostream>
#include <string>
#include <sstream>
#include "logging.h"
#include <iostream>
#include <vector>
using namespace std;


Sequencer::Sequencer(OutPin& out) :
  out(&out), origvalue(0), pattern(nullptr), startTime(0), activeStep(0) {}

Sequencer::~Sequencer() {}

bool Sequencer::isRunning() {
  return pattern != NULL;
}

SeqPattern * Sequencer::stop(bool restore) {
  SeqPattern *old = pattern;

  if (isRunning()) {
    if (restore) {
      COUT_DEBUG(cout << "stopping, back to value " << origvalue << endl);
      out->write(origvalue);
    } else {
      out->write(0);
    }
  }
  pat_next = nullptr;
  pattern  = nullptr;
  done.emit();
  return old;
}

void Sequencer::endPattern(int value) {
  // decrement positive repeatcounts
  if (repeatcount > 0) repeatcount--;

  if (repeatcount == 0) {
    COUT_DEBUG(Serial.println("stop repeat"));

    if (pat_next && *pat_next) {
      pattern = *pat_next;
      pat_next++;
    } else {
      stop();
      return;
    }
  }

  COUT_DEBUG(Serial.print("remaining repeatcount "));
  COUT_DEBUG(Serial.println(repeatcount));

  this->startTime  = millis();
  this->activeStep = 0;
  activate();
}

void Sequencer::printStep(struct SeqElement& step) {
  COUT_DEBUG(
    cout << activeStep << "[" << step.duration << " ; " << step.value << "]" <<
      endl);
}

void Sequencer::activate() {
  SeqElement& el = pattern->elements[activeStep];

  COUT_DEBUG(cout << "next step ");
  printStep(el);

  if (!el.value && (el.duration == 0)) {
    endPattern(el.value);
  } else {
    COUT_DEBUG(cout << "write: " << el.value << endl);
    out->write(el.value);
  }
}

void Sequencer::handle() {
  if (!this->pattern) return;

  SeqElement& step = pattern->elements[activeStep];

  if ((unsigned long)millis() - this->startTime > step.duration) {
    COUT_DEBUG(
      cout << " " << millis() << " \t -  " << this->startTime << " \t= " <<
      ((unsigned long)millis() - this->startTime) << endl);
    SeqElement& elold = pattern->elements[activeStep];
    COUT_DEBUG(cout << "executed step ");
    printStep(elold);
    startTime += elold.duration;
    activeStep++;
    activate();
  }
}

void Sequencer::_start(SeqPattern *pattern) {
  this->pattern = pattern;
  startTime     = millis();
  activeStep    = 0;
  origvalue     = out->read();
  repeatcount   = pattern->repeatcount;
  activate();
  handle();
}

void Sequencer::start(SeqPattern *pattern) {
  pat_next = NULL;
  _start(pattern);
}

void Sequencer::startSeries(SeqPattern **series) {
  if (!series || !*series) return;

  pattern  = *series;
  pat_next = ++series;
  _start(pattern);
}

/* create a pattern from a string
 * sequence of <ms>*<value> separated by spaces*/
SeqPattern * Sequencer::createPattern(std::string pat) {
  std::istringstream is(pat);
  std::vector<SeqElement> elements;
  SeqPattern *result = new SeqPattern();
  std::string seqstr;

  /* arduino ulibc++ getline does not return false when done !*/
  while (std::getline(is, seqstr, ' ') && seqstr.length() != 0) {
    char dummy;
    SeqElement el;

    //		cout << "process \"" << seqstr << "\"" <<endl;
    std::istringstream is2(seqstr);
    is2 >> el.duration;
    is2 >> dummy; // read '*'
    is2 >> el.value;
    elements.push_back(el);
  }
  SeqElement el = { 0, false };
  elements.push_back(el);

  result->elements = new SeqElement[elements.size()];
  copy(elements.begin(), elements.end(), result->elements);

  result->repeatcount = 1;

  return result;
}
