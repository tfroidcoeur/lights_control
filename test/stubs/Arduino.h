/*
 * Arduino.h
 *
 *  Created on: Dec 31, 2017
 *      Author: fraco
 */

#include <stddef.h>
#include <stdint.h>
#include <iostream>

#ifndef STUBS_ARDUINO_H_
#define STUBS_ARDUINO_H_

/* definitions copied from Arduino.h */
#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

void pinReset();
void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);

unsigned long millis(void);
using namespace std;
typedef bool boolean;
typedef uint8_t byte;

/* adapted from Arduino SDK */

#define DEC 10

class HardwareSerial {
public:
	HardwareSerial() {
	}

//    void clearWriteError() { setWriteError(0); }
//
//    virtual size_t write(uint8_t) = 0;
//    size_t write(const char *str) {
//      if (str == NULL) return 0;
//      return write((const uint8_t *)str, strlen(str));
//    }
//    virtual size_t write(const uint8_t *buffer, size_t size);
//    size_t write(const char *buffer, size_t size) {
//      return write((const uint8_t *)buffer, size);
//    }
//
//    // default to zero, meaning "a single write may block"
//    // should be overriden by subclasses with buffering
//    virtual int availableForWrite() { return 0; }
//
//    size_t print(const __FlashStringHelper *);
//    size_t print(const String &);
	size_t print(const char a[]) {
		cout << a;
		return 1;
	}
//    size_t print(char);
//    size_t print(unsigned char, int = DEC);
	size_t print(int val, int base = DEC) {
		cout << val;
		return 1;
	}
//    size_t print(unsigned int, int = DEC);
//    size_t print(long, int = DEC);
	size_t print(unsigned long val, int base = DEC) {
		cout << val;
		return 1;
	}
//    size_t print(double, int = 2);
//    size_t print(const Printable&);
//
//    size_t println(const __FlashStringHelper *);
//    size_t println(const String &s);
	size_t println(const char a[]) {
		cout << a << endl;
		return 1;
	}
//    size_t println(char);
//    size_t println(unsigned char, int = DEC);
	size_t println(int val, int base = DEC) {
		cout << val << endl;
		return 1;
	}
	size_t println(unsigned int val, int base = DEC) {
		cout << val << endl;
		return 1;
	}
//    size_t println(long, int = DEC);
	size_t println(unsigned long val, int base = DEC) {
		cout << val << endl;
		return 1;
	}
//    size_t println(double, int = 2);
//    size_t println(const Printable&);
    size_t println(void){
		cout << endl;
		return 1;
	}
//
//    virtual void flush() { /* Empty implementation for backward compatibility */ }
	void begin(int baud) {
	}
	;
};

#define F(a) a

extern HardwareSerial Serial;

#endif /* STUBS_ARDUINO_H_ */
