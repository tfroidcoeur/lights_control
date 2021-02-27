/*
 * Stashable.h
 *
 *  Created on: Apr 11, 2018
 *      Author: fraco
 */

#ifndef STASHABLE_H_
#define STASHABLE_H_

class Stashable {
public:

  Stashable() {}

  virtual ~Stashable() {}

  virtual void save()    = 0;
  virtual void restore() = 0;
};

#endif /* STASHABLE_H_ */
