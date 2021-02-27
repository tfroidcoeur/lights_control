#ifndef MQTTROOT_H_
#define MQTTROOT_H_
#include <map>
#include <string>
#include <iterator>
#include <memory>
using namespace std;
#include "MqttNode.h"
#include <MQTT.h>
#include <Ethernet.h>
#include "Actor.h"
#include "logging.h"
#include <MemoryFree.h>


class MqttHandler {
public:

  virtual void messageReceived(String& topic,
                               String& payload) = 0;
};

class MqttRoot : public MqttNode, public Actor, public MqttHandler {
public:

  // MqttNode
  MqttRoot(string name = "Controllino");
  ~MqttRoot() {}

  // Actor

  void setup() {
    /* thats right, you blocking network stack, get connected in 20ms then*/
    net.setConnectionTimeout(20);
    mqttclient.begin("10.0.0.202", 1883, net);
    mqttclient.setOptions(30, true, 100);
    mqttclient.onMessage(__messageReceived);
  }

  void handle() {
    string vpath = string("home/") + name + "/version";

    if (!mqttclient.connected()) {
      if (millis() - lastConnectAttempt > connectRetryTime) {
        mqttclient.connect(name.c_str());
        lastConnectAttempt = millis();

        if (mqttclient.connected()) {
          Serial.println("connected mqt");
          publish(vpath, string(VERSION));
          refresh();
        }
      }
    } else  {
      // update lastConnectAttempt to make sure we don't hit the wrap
      // around and will try to reconnect after timeout
      lastConnectAttempt = millis();

      // Serial.println("loop mqtt");
      mqttclient.loop();
    }
  }

  // MqttNode
  virtual void subscribe(string const& path) {
    COUT_DEBUG(cout << "Subscribe " << path << endl);
    mqttclient.subscribe(path.c_str());
  }

  virtual void publish(string const& path, string const& value) {
    COUT_DEBUG(cout << "Publish " << path << " = " << value << endl);
    mqttclient.publish(path.c_str(), value.c_str(), true, 0);
  }

  virtual void update(string const& path, string const& value) {
    if (child) {
      child->update(path, value);
    }
  }

  static void __messageReceived(String& topic,
                                String& payload);

  void        messageReceived(String& topic, String& payload) {
      #ifdef DEBUG
    string msg = string("incoming: ") + topic.c_str() + " - " + payload.c_str();
    Serial.println(msg.c_str());
      #endif // ifdef DEBUG
    child->update(topic.c_str(), payload.c_str());
  }

  void setChild(MqttNode *child) {
    this->child = child;
  }

  void refresh() {
    if (child) {
      COUT_DEBUG(cout << "refresh " << child->getName() << endl);
      child->refresh();
    }
  }

private:

  string name;
  MqttNode  *child;
  MQTTClient mqttclient;
  EthernetClient net;
  unsigned long  lastConnectAttempt = 0;
  unsigned long  connectRetryTime   = 10000;
};

#endif /* MQTTROOT_H */
