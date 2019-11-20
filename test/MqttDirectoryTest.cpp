#define DEBUG

#include "MqttDirectoryTest.h"
#include "Controllino.h"
#include "MqttDirectory.h"
#include "MqttNode.h"
#include <cpptest-assert.h>

MqttDirectoryTest::MqttDirectoryTest() {
  TEST_ADD(MqttDirectoryTest::testPublish);
  TEST_ADD(MqttDirectoryTest::testSubscribe);
  TEST_ADD(MqttDirectoryTest::testUpdate);
}

MqttDirectoryTest::~MqttDirectoryTest() {}

class TestMqttNode : public MqttNode {

public:
  TestMqttNode(string name) : MqttNode(name, NULL) {}
  ~TestMqttNode() {}
  /* child node asks parent node to subscribe to the sub path provided
     this ripples up the tree until it reaches the root */
  void subscribe(string const &path) { lastsubpath = path; }
  /* child node asks parent node to publish updated value */
  void publish(string const &path, string const &value) {
    lastpubpath = path;
    lastpubval = value;
  }
  /* parent notifies child of a publish, sub path provided */
  void update(string const &path, string const &value) {
    lastuppath = path;
    lastupval = value;
  }
  string lastsubpath;
  string lastpubpath;
  string lastuppath;
  string lastpubval;
  string lastupval;
};

void MqttDirectoryTest::setup() {}

void MqttDirectoryTest::tear_down() {}

void MqttDirectoryTest::testUpdate() {
  TestMqttNode root("root");
  TestMqttNode child1("child1");
  TestMqttNode child2("child2");

  MqttDirectory dir("mydir", &root);
  dir.addNode(&child1);
  dir.addNode(&child2);

  dir.update("child1/pad/ding", "hello child1");
  dir.update("child2/pad/ding/dong", "hello child2");
  dir.update("someotherchild/pad/ding/dong", "hello child none");

  TEST_ASSERT_EQUALS("pad/ding",child1.lastuppath);
  TEST_ASSERT_EQUALS("pad/ding/dong",child2.lastuppath);
  TEST_ASSERT_EQUALS("hello child1", child1.lastupval);
  TEST_ASSERT_EQUALS("hello child2",child2.lastupval);

}

void MqttDirectoryTest::testPublish() {
  TestMqttNode root("root");
  TestMqttNode child1("child1");
  TestMqttNode child2("child2");

  MqttDirectory dir("mydir", &root);
  dir.addNode(&child1);
  dir.addNode(&child2);

  dir.publish("child1/my/sub/path", "my value");
  TEST_ASSERT_EQUALS("mydir/child1/my/sub/path", root.lastpubpath);

}

void MqttDirectoryTest::testSubscribe() {
  TestMqttNode root("root");
  TestMqttNode child1("child1");
  TestMqttNode child2("child2");

  MqttDirectory dir("mydir", &root);
  dir.addNode(&child1);
  dir.addNode(&child2);

  dir.subscribe("sub/path");
  TEST_ASSERT_EQUALS("mydir/sub/path", root.lastsubpath);

}
