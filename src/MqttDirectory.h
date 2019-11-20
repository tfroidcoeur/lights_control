#ifndef MQTTDIRECTORY_H_
#define MQTTDIRECTORY_H_
#include <map>
#include <string>
#include <iterator>
#include <memory>
using namespace std;
#include "MqttNode.h"

/* design options:
   subscribe to whole tree, but this means we could get flooded if some other device
   uses the same tree
   or subscribe to the relevant parts of the tree only (maybe best)
   --> only subscribe at the leaves */
/* design options:
   ripple and publish requests up through the tree until the top one or
   directly publish from the leaf node
   probably best to ripple up, so we don't have to pass configuration down to in the tree
   */
class MqttDirectory: public MqttNode{
public:
   // MqttNode
	MqttDirectory( string const name, MqttNode * parent):
      MqttNode(name, parent) {

      }
	~MqttDirectory() {
   }

   void addNode(MqttNode * child) {
      if (children[child->getName()])
         delete children[child->getName()];
      children[child->getName()] = child;
   }
	virtual void subscribe(string const& path) {
      // add our part of the path and call parent
      if (parent)
         parent->subscribe(name + "/" + path);
   }
	virtual void update(string const& path, string const & value) {
      MqttNode *  n;
      string remainpath;
      // if path is empty, skip
      if (path.size() == 0)
         return;

      // split off first part of the path
      std::size_t slashpos;
      slashpos=path.find('/');
      if (std::string::npos == slashpos) {
         n = children[path];
         remainpath = "";
      } else {
         n=children[path.substr(0, slashpos)];
         remainpath = path.substr(slashpos+1, string::npos);
      }
      if (n) {
         n->update(remainpath, value);
      }
   }
	virtual void publish(string const& path, string const & value) {
      if (parent)
         parent->publish(name + "/" + path, value);
   }
private:
   std::map<string, MqttNode*> children;
};

#endif /* MQTTDIRECTORY_H */
