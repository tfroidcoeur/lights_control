/*
 * logging.h
 *
 *  Created on: Mar 19, 2018
 *      Author: fraco
 */

#include <ArduinoSTL.h>
using namespace std;
#undef COUT_DEBUG
#undef COUT_INFO
#undef COUT_WARN
#ifdef DEBUG
#define COUT_DEBUG(a) a
#define COUT_INFO(a) a
#define COUT_WARN(a) a
#else
#define COUT_DEBUG(a)
#define COUT_INFO(a) a
#define COUT_WARN(a) a
#endif
