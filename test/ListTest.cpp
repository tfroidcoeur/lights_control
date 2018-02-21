/*
 * ListTest.cpp
 *
 *  Created on: Feb 21, 2018
 *      Author: fraco
 */

#include "ListTest.h"
#include <List.h>
#include "cpptest.h"
#include "cpptest-assert.h"

class Dummy {
};

void ListTest::add() {
	List<Dummy> l;
	Dummy one;
	Dummy two;
	l.add(one);
	l.add(two);

	TEST_ASSERT(l.remove(one));
	TEST_ASSERT(l.remove(two));
}

void ListTest::iterate() {
	List<Dummy> l;
	Dummy one;
	Dummy two;
	l.add(one);
	l.add(two);
	int count=0;

	List<Dummy>::iterator it = l.begin();
	for (; it!=l.end(); it++) count++;

	TEST_ASSERT(count ==2);
}
