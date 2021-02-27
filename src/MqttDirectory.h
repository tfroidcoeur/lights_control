#ifndef MQTTDIRECTORY_H_
#define MQTTDIRECTORY_H_
#include <map>
#include <string>
#include <iterator>
#include <memory>
#include "logging.h"
#include <iostream>
using namespace std;
#include "MqttNode.h"

/* design options:
   subscribe to whole tree, but this means we could get flooded if some other
      device
   uses the same tree
   or subscribe to the relevant parts of the tree only (maybe best)
   --> only subscribe at the leaves */

/* design options:
   ripple and publish requests up through the tree until the top one or
   directly publish from the leaf node
   probably best to ripple up, so we don't have to pass configuration down to in
      the tree
 */
class MqttDirectory : public MqttNode {
public:

  // MqttNode
  MqttDirectory(const char *name, MqttNode *parent) :
    MqttNode(name, parent) {}

  ~MqttDirectory() {}

  void addNode(MqttNode *child) {
    string name(child->getName());

    COUT_DEBUG(cout << "add child " << name << endl);

    if (children[name]) delete children[name];
    children[name] = child;
  }

  virtual void subscribe(string const& path) {
    // add our part of the path and call parent
    if (parent) parent->subscribe(string(name) + "/" + path);
  }

  virtual void update(string const& path, string const& value) {
    MqttNode *n;
    string    us;
    string    childn;
    string    remainpath;

    COUT_DEBUG(cout << "find path " << path << endl);

    // if path is empty, skip
    if (path.size() == 0) return;

    // us/child/further or
    // us/child or
    // us

    // split off first part of the path
    std::size_t slashpos;
    slashpos = path.find('/');

    // no child, don't care
    if (std::string::npos == slashpos) return;

    // not us
    us = path.substr(0, slashpos);

    if (us != name) return;

    // child/remain
    remainpath = path.substr(slashpos + 1, string::npos);
    slashpos   = remainpath.find('/');

    if (std::string::npos == slashpos) {
      childn     = remainpath;
      remainpath = "";
    } else {
      childn     = remainpath.substr(0, slashpos);
      remainpath = remainpath.substr(slashpos + 1, string::npos);
    }
    n = children[childn];

    if (n) {
      COUT_DEBUG(cout << "found path " << path << endl);
      n->update(remainpath, value);
    }
  }

  virtual void publish(string const& path, string const& value) {
    if (parent) parent->publish(string(name) + "/" + path, value);
  }

  virtual void refresh() {
    for (auto const& x : children) {
      COUT_DEBUG(cout << "refresh " << x.second->getName() << endl);
      x.second->refresh();
    }
  }

private:

  std::map<string, MqttNode *>children;
};

#endif /* MQTTDIRECTORY_H */
