#ifndef MQTTDIRECTORYTEST_H_
#define MQTTDIRECTORYTEST_H_

#include <cpptest-suite.h>
#include <sigslot.h>

#include "TestWithTime.h"
#include "Sequencer.h"
#include "OutPin.h"
#include "Controllino.h"


class MqttDirectoryTest : public Test::Suite {
public:

  MqttDirectoryTest();
  virtual ~MqttDirectoryTest();
  void testPublish();
  void testUpdate();
  void testSubscribe();

protected:

  void setup();
  void tear_down();
};

#endif /* MQTTDIRECTORYTEST_H_ */
