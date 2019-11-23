#include "MqttRoot.h"
#include "Arduino.h"
#include "ArduinoSTL.h"
//#define DEBUG
#include "logging.h"
using namespace std;

static MqttHandler *r;

void MqttRoot::__messageReceived(String &topic, String &payload) {
  if (r)
        r->messageReceived(topic, payload);
  else {
            Serial.println("no r for mqtt");
  }
};

MqttRoot::MqttRoot() : MqttNode(name) { r = this; };
