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


class MqttHandler {
   public:
      virtual void messageReceived(String &topic, String &payload)=0;
};

class MqttRoot: public MqttNode, public Actor, public MqttHandler{
public:
   // MqttNode
	MqttRoot();
	~MqttRoot() {};

   // Actor

   void setup() {
      Serial.println("setup mqtt");
      /* thats right, you blocking network stack, get connected in 20ms then*/
      net.setConnectionTimeout(20);
      mqttclient.begin("10.0.0.202", 1883, net);

      mqttclient.onMessage(__messageReceived);
   }

   void handle() {
      if (!mqttclient.connected()) {
         // Serial.println("connect mqt");
         mqttclient.connect("Controllino");
         if (mqttclient.connected()) {
            Serial.println("connected mqt");
            mqttclient.subscribe("controllino/hello");
            // Serial.println("subscribed mqt");
         }
      } else {
            // Serial.println("loop mqtt");
         mqttclient.loop();
      }
   }

   // MqttNode
	virtual void subscribe(string const& path) {
   }
	virtual void publish(string const& path, string const & value) {
   }

	virtual void update(string const& path, string const & value){
      if (child) {
         child->update(path, value);
      }
   }

   static void __messageReceived(String &topic, String &payload);

   void messageReceived(String &topic, String &payload) {
      string msg = string("incoming: ") + topic.c_str() + " - " + payload.c_str();
      Serial.println(msg.c_str());
      mqttclient.publish("controllino/hello_back", "pong");
   };

  void setChild(MqttNode * child) {this->child = child;}
private:
   MqttNode * child;
   MQTTClient mqttclient;
   EthernetClient net;
};

#endif /* MQTTROOT_H */
