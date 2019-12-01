/*
 * Teleruptor.h
 *
 *  Created on: Dec 16, 2017
 *      Author: fraco
 */
#include <map>
#include <string>
#include <iterator>
#include <iostream>
using namespace std;

#ifndef MQTTNODE_H_
#define MQTTNODE_H_

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
class MqttNode {
public:
	MqttNode( string const name, MqttNode * parent): name(name), parent(parent){}
	MqttNode( string const name): name(name), parent(NULL){}
	MqttNode(const MqttNode & orig): name(orig.name), parent(orig.parent){}
	virtual ~MqttNode(){};
	/* child node asks parent node to subscribe to the sub path provided
	   this ripples up the tree until it reaches the root */
	virtual void subscribe(string const& path){
     if (parent)
      parent->subscribe(path);
   };
   /* child node asks parent node to publish updated value */
	virtual void publish(string const& path, string const & value){
     if (parent)
      parent->publish(path, value);
   };
   /* re-subscribe and publish state */
   virtual void refresh()=0;
	/* parent notifies child of a publish, sub path provided */
	virtual void update(string const& path, string const & value)=0;
   string const &getName() { return name; }
   void setParent(MqttNode * parent) { this->parent = parent;}
protected:
   string const name;
   MqttNode * parent;

};

#endif /* MQTTNODE_H */
