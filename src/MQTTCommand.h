#ifndef MQTTCOMMAND_H_
#define MQTTCOMMAND_H_

#include "sigslot.h"
#include "Switchable.h"
#include "MqttRoot.h"

class MQTTCommand : public sigslot::has_slots<> {
public:

  MQTTCommand(const char *path,
              const char *value,
              MqttRoot   *mqtt) : path(path), value(value), mqtt(mqtt) {}

  ~MQTTCommand() {}

  void doit() {
    mqtt->publish(path, value);
  }

private:

  const char *path;
  const char *value;
  MqttRoot *mqtt;
};

#endif /* MQTTCOMMAND_H_ */
