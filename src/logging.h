/*
 * logging.h
 *
 *  Created on: Mar 19, 2018
 *      Author: fraco
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#ifdef DEBUG
#define COUT_DEBUG(a) a
#define COUT_INFO(a) a
#define COUT_WARN(a) (a)
#else
#define COUT_DEBUG(a)
#define COUT_INFO(a) a
#define COUT_WARN(a) a
#endif



#endif /* LOGGING_H_ */
