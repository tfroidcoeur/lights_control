/*
 * Staircase light. Staircase light is timer based light.
 * Will stay on for predefined time, then blink slowly three times and go off.
 * Time depends on short/longpress.
 * short: 2 minutes
 * long: 1 hour
 * blinks are of duration 500ms and 10 seconds apart.
 */

#ifndef STAIRCASE_H_
#define STAIRCASE_H_

#include "Actor.h"
#include "Button.h"
#include "InPin.h"
#include "MqttNode.h"
#include "OutPin.h"
#include "Sequencer.h"
#include "sigslot.h"

using namespace std;

class Staircase : public MqttNode, public Actor, public sigslot::has_slots<> {
public:

  virtual ~Staircase();
  Staircase(const Staircase& orig) : MqttNode(orig), seq(orig.seq) {}

  Staircase(OutPin    & outpin,
            const char *name,
            MqttNode   *parent = NULL);
  Staircase(sigslot::signal0<>& shortsig,
            OutPin             *outpin,
            const char         *name,
            MqttNode           *parent = NULL);
  void         shortpressed();
  void         longpressed();

  /* Switchable */
  virtual void on();
  virtual void off();
  virtual bool isOn();

  /* Actor */
  void         setup();
  void         handle();

  // mqtt node
  virtual void update(string const& path,
                      string const& value);
  virtual void refresh();

private:

  static SeqPattern startBlink;
  static SeqPattern endBlink;
  static SeqPattern normalLight;
  static SeqPattern longLight;
  OutPin *outpin;
  Sequencer seq;
  SeqPattern *pat[3];
  void start(SeqPattern& c);
  void publishUpdate();
};

#endif /* STAIRCASE_H_ */
