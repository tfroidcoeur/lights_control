/*
 * IPAddress.h
 *
 *  Created on: Mar 10, 2019
 *      Author: fraco
 */

#ifndef STUBS_IPADDRESS_H_
#define STUBS_IPADDRESS_H_
#include <stdint.h>

class IPAddress {
public:

  // Constructors
  IPAddress();
  IPAddress(uint8_t first_octet,
            uint8_t second_octet,
            uint8_t third_octet,
            uint8_t fourth_octet) {}

  IPAddress(uint32_t address) {}

  IPAddress(const uint8_t *address) {}
};

#endif /* STUBS_IPADDRESS_H_ */
