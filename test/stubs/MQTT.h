#include <Arduino.h>
#include <Ethernet.h>
#include <memory>
typedef void (*MQTTClientCallbackSimple)(String &topic, String &payload);

class MQTTClient {
public:
  bool publish(const String &topic, const String &payload) { return true; }
  bool connected() { return true; };
  bool connect(const String &topic) { return true; }
  bool subscribe(const String &topic) { return true; }
  bool loop(){ return true;}
  void begin(const char hostname[], Client &client) {  }
  void begin(const char hostname[], int port, Client &client) {};
  void onMessage(MQTTClientCallbackSimple cb){}
};
