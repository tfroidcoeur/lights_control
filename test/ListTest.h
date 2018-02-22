/*
 * ListTest.h
 *
 *  Created on: Feb 21, 2018
 *      Author: fraco
 */

#ifndef LISTTEST_H_
#define LISTTEST_H_
#include <cpptest.h>

class ListTest : public Test::Suite
{
public:
	ListTest()
	{
		TEST_ADD(ListTest::add);
		TEST_ADD(ListTest::iterate);
		TEST_ADD(ListTest::iterate2);
	}

private:
	void add();
	void iterate();
	void iterate2();
};

#endif /* LISTTEST_H_ */
