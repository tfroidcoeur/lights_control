#include "MqttRoot.h"
#include "Arduino.h"
#include "ArduinoSTL.h"

// #define DEBUG
#include "logging.h"
using namespace std;

static MqttHandler *r;

void MqttRoot::__messageReceived(String& topic, String& payload) {
  if (r)  r->messageReceived(topic, payload);
}

MqttRoot::MqttRoot(string name) : MqttNode("<root>"), name(name) {
  lastConnectAttempt = millis() - connectRetryTime;
  r                  = this;
}
