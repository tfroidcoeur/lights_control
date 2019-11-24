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
      virtual void messageReceived(String &topic, String &payload)=0;
};

class MqttRoot: public MqttNode, public Actor, public MqttHandler{
public:
   // MqttNode
	MqttRoot();
	~MqttRoot(){};

   // Actor

   void setup() {
      /* thats right, you blocking network stack, get connected in 20ms then*/
      net.setConnectionTimeout(20);
      mqttclient.begin("10.0.0.202", 1883, net);
      mqttclient.setOptions(30, true, 100);
      mqttclient.onMessage(__messageReceived);
   }

   void handle() {
      if (!mqttclient.connected()){
         if (millis() - lastConnectAttempt > connectRetryTime) {
            mqttclient.connect("Controllino");
            lastConnectAttempt=millis();
            if (mqttclient.connected()) {
               Serial.println("connected mqt");
               refresh();
            }
         }
      } else  {
         // update lastConnectAttempt to make sure we don't hit the wrap
         // around and will try to reconnect after timeout
         lastConnectAttempt=millis();
            // Serial.println("loop mqtt");
         mqttclient.loop();
      }
   }

   // MqttNode
	virtual void subscribe(string const& path) {
      cout << "Subscribe " << path << endl;
      mqttclient.subscribe(path.c_str());
   }
	virtual void publish(string const& path, string const & value) {
      cout << "Publish " << path << " = " << value << endl;
      mqttclient.publish(path.c_str(),value.c_str(), true, 0);
   }

	virtual void update(string const& path, string const & value){
      if (child) {
         child->update(path, value);
      }
   }

   static void __messageReceived(String &topic, String &payload);

   void messageReceived(String &topic, String &payload) {
      cout << "free: " << freeMemory() <<endl;
      string msg = string("incoming: ") + topic.c_str() + " - " + payload.c_str();
      Serial.println(msg.c_str());
      child->update(topic.c_str(), payload.c_str());
   };

  void setChild(MqttNode * child) {this->child = child;}
  void refresh(){
      if (child) {
         cout << "refresh " << child->getName() << endl;
         child->refresh();
      }
  }
private:
   MqttNode * child;
   MQTTClient mqttclient;
   EthernetClient net;
   unsigned long lastConnectAttempt=0;
   unsigned long connectRetryTime=10000;
};

#endif /* MQTTROOT_H */
