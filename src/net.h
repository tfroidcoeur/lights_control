
#ifndef NET_H_
#define NET_H_
#include "Actor.h"

class Net : public Actor {
public:

  virtual ~Net() {}

  /**
   * method called regularly in the loop
   */
  void handle();

  /**
   * setup function
   */
  void setup();
};

#endif /* NET_H_ */
