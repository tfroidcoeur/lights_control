// ---
//
// $Id: mytest.cpp,v 1.5 2008/07/11 16:49:43 hartwork Exp $
//
// CppTest - A C++ Unit Testing Framework
// Copyright (c) 2003 Niklas Lundell
//
// ---
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the
// Free Software Foundation, Inc., 59 Temple Place - Suite 330,
// Boston, MA 02111-1307, USA.
//
// ---
//
// Test program demonstrating all assert types and output handlers.
//
// ---

#include <Arduino.h>
#include <cpptest-assert.h>
#include <cpptest-compileroutput.h>
#include <cpptest-htmloutput.h>
#include <cpptest-suite.h>
#include <cpptest-textoutput.h>
#include <Controllino.h>
#include <InPin.h>
#include <sigslot.h>
#include <string.h>
#include <Time.h>
#include <cstdlib>
#include <iostream>

#include "ButtonTest.h"
#include "DebouncedInput.h"
#include "DimmerTest.h"
#include "Input.h"
#include "MotionSpotTest.h"
#include "ControllerTest.h"
#include "SequencerTest.h"
#include "MqttDirectoryTest.h"

class MotionSpotTest;

#ifdef _MSC_VER
	#pragma warning (disable: 4290)
#endif

#include "cpptest.h"

using namespace std;

// Test InPin behaviour
class InPinTest : public Test::Suite, public sigslot::has_slots<>, public TestWithTime
{
public:
	InPinTest()
	{
		TEST_ADD(InPinTest::debounceTest)

	}
protected:
	// just count number of notifications
	virtual void notify(int value){
		notified++;
	}

	void setup(){
		TestWithTime::setup();
		pinReset();
	}

	void tear_down(){
		TestWithTime::tear_down();
	}

private:
	int notified=0;
	// Test debounce behaviour
	void debounceTest() {
		// use our own time, we can play with
		Time &t=*time;

		// pin under test
		InPin * pin = new InPin (CONTROLLINO_A1);
		DebouncedInput p(pin);
		p.getChangeSignal().connect(this, &InPinTest::notify);
		p.setup();

		// run the pin a first time
		p.handle();

		// and again 500 ms later
		t+=500;
		p.handle();

		TEST_ASSERT(notified==0);


		// now activate the input pin
		digitalWrite(CONTROLLINO_A1,1);

		// the pin should not be notfied before it has been stable for 20 ms
		p.handle();
		t+=18;
		p.handle();
		TEST_ASSERT(notified==0);

		t++;
		p.handle();
		TEST_ASSERT(notified==0);

		// bring it down before it actually triggers
		// this whole episode should not be a trigger
		digitalWrite(CONTROLLINO_A1,0);
		p.handle();
		t++;
		p.handle();
		TEST_ASSERT(notified==0);

		// pin active again
		digitalWrite(CONTROLLINO_A1,1);
		p.handle();
		t+=18;
		p.handle();
		TEST_ASSERT(notified==0);
		t+=3;
		p.handle();
		// after 21 ms, finally, the pin should have notified
		TEST_ASSERT(notified==1);

		// and check if it notifies off also
		digitalWrite(CONTROLLINO_A1,0);
		p.handle();
		t+=22;
		p.handle();
		TEST_ASSERT(notified==2);
	}
};

enum OutputType
{
	Compiler,
	Html,
	TextTerse,
	TextVerbose
};

static void
usage()
{
	cout << "usage: mytest [MODE]\n"
		 << "where MODE may be one of:\n"
		 << "  --compiler\n"
		 << "  --html\n"
		 << "  --text-terse (default)\n"
		 << "  --text-verbose\n";
	exit(0);
}

static auto_ptr<Test::Output>
cmdline(int argc, char* argv[])
{
	if (argc > 2)
		usage(); // will not return

	Test::Output* output = 0;

	if (argc == 1)
		output = new Test::TextOutput(Test::TextOutput::Verbose);
	else
	{
		const char* arg = argv[1];
		if (strcmp(arg, "--compiler") == 0)
			output = new Test::CompilerOutput;
		else if (strcmp(arg, "--html") == 0)
			output =  new Test::HtmlOutput;
		else if (strcmp(arg, "--text-terse") == 0)
			output = new Test::TextOutput(Test::TextOutput::Terse);
		else if (strcmp(arg, "--text-verbose") == 0)
			output = new Test::TextOutput(Test::TextOutput::Verbose);
		else
		{
			cout << "invalid commandline argument: " << arg << endl;
			usage(); // will not return
		}
	}

	return auto_ptr<Test::Output>(output);
}

// Main test program
//
int
main(int argc, char* argv[])
{
	try
	{
		// Demonstrates the ability to use multiple test suites
		//
		Test::Suite ts;
		ts.add(auto_ptr<Test::Suite>(new InPinTest));
		ts.add(auto_ptr<Test::Suite>(new ButtonTest));
		ts.add(auto_ptr<Test::Suite>(new MotionSpotTest));
		ts.add(auto_ptr<Test::Suite>(new ControllerTest));
		ts.add(auto_ptr<Test::Suite>(new SequencerTest));
		ts.add(auto_ptr<Test::Suite>(new MqttDirectoryTest));
		ts.add(auto_ptr<Test::Suite>(new DimmerTest));

		// Run the tests
		//
		auto_ptr<Test::Output> output(cmdline(argc, argv));
		ts.run(*output, true);

		Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
		if (html)
			html->generate(cout, true, "MyTest");
	}
	catch (...)
	{
		cout << "unexpected exception encountered\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
