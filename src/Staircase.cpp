/*
 */

#include "Staircase.h"
#include "OutPin.h"
#include "Sequencer.h"
#include <limits.h>

// #define DEBUG

// #undef DEBUG
#include "logging.h"
using namespace std;

#define SECOND 1000L
#define MINUTE (60 * SECOND)
#define HOUR   (60 * MINUTE)

/* timer patterns of the lights */
SeqPattern Staircase::startBlink(0, new SeqElement[2]  {
  { 500, false },
  { 0, false }
});

SeqPattern Staircase::endBlink(0, new SeqElement[7]  {
  /* blink 3 times every 10 seconds, then off */
  { 500, false },
  { 10 * SECOND, true },
  { 500, false },
  { 10 * SECOND, true },
  { 500, false },
  { 10 * SECOND, true },
  { 0, false }
});

SeqPattern Staircase::normalLight(0, new SeqElement[2]  {
  { 5 * MINUTE, true },
  { 0, false }
});

SeqPattern Staircase::longLight(0, new SeqElement[2]  {
  { HOUR, true },
  { 0, false }
});

Staircase::~Staircase() {
  //	cout << "~mspto " << hex << this <<endl;
}

Staircase::Staircase(OutPin    & pin,
                     const char *name,
                     MqttNode   *parent) :
  MqttNode(name, parent), outpin(&pin), seq(pin)  {
  //	cout << "mspto " << hex << this <<endl;
  seq.done.connect(this, &Staircase::sequencedone);
}

Staircase::Staircase(sigslot::signal0<>& shortsig,
                     OutPin             *outpin,
                     const char         *name,
                     MqttNode           *parent) :
  MqttNode(name, parent), outpin(outpin), seq(*outpin) {
  shortsig.connect(this, &Staircase::shortpressed);
  seq.done.connect(this, &Staircase::sequencedone);
}

void Staircase::handle() {
  // button and input pin handling will notify on state change
  seq.handle();
}

void Staircase::setup() {
  seq.setup();
}

void Staircase::start(SeqPattern& c) {
  SeqPattern **p = &pat[0];

  if (seq.isRunning()) {
    seq.stop();
    *p++ = &startBlink;
  }
  *p++ = &c;
  *p++ = &endBlink;
  *p++ = NULL;

  seq.startSeries(pat);
  publishUpdate();
}

void Staircase::stop() {
  if (isOn()) {
    seq.stop();
    outpin->write(0);
    publishUpdate();
  }
}

void Staircase::shortpressed() {
  COUT_DEBUG(cout << "staircase short press " << (isOn() ? "ON" : "OFF") << endl);

  if (isOn()) stop();
  else start(normalLight);
}

void Staircase::longpressed() {
  COUT_DEBUG(cout << "staircase long press " << (isOn() ? "ON" : "OFF") << endl);
  start(longLight);
}

void Staircase::sequencedone() {
  COUT_DEBUG(cout << "staircase seq done" << (isOn() ? "ON" : "OFF") << endl);
  publishUpdate();
}

void Staircase::on() {
  if (!isOn()) start(normalLight);
}

void Staircase::off() {
  if (isOn()) {
    stop();
  }
}

bool Staircase::isOn() {
  return seq.isRunning();
}

void Staircase::publishUpdate() {
  publish(string(name) + "/state", isOn() ? "ON" : "OFF");
}

void Staircase::update(string const& path, string const& value) {
  COUT_DEBUG(cout << "staircase mqtt " << path << " " << value << endl);

  if (value.compare("ON") == 0) on();
  else if (value.compare("OFF") == 0) off();
}

void Staircase::refresh() {
  COUT_DEBUG(cout << "refresh " << name << endl);
  subscribe(string(name) + "/control");
}
