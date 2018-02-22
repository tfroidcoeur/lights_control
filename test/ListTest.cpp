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

void ListTest::iterate2() {
	List<Dummy> l;
	Dummy * lost;
	bool found=false;

	for (int i=0; i<100; i++) {
		Dummy * tmp = new Dummy();
		l.add(tmp);
		if (i==34) lost=tmp;
	}

	int count=0;

	List<Dummy>::iterator it = l.begin();
	for (; it!=l.end(); it++) {
		count++;
		if (&(*it)==lost) found=true;
	}

	TEST_ASSERT(count ==100);
	TEST_ASSERT(found);
	TEST_ASSERT(l.find(*lost));
	while (!l.empty()){
		l.remove(*l.begin());
	}
}
