/*
 * Ethernet.h
 *
 *  Created on: Mar 10, 2019
 *      Author: fraco
 */
#include "IPAddress.h"
#include "Udp.h"

#ifndef ETHERNET_H_
# define ETHERNET_H_

class Client {};
class EthernetClass {
public:

  int begin(uint8_t      *mac_address,
            unsigned long timeout         = 60000,
            unsigned long responseTimeout = 4000) {
    return 1;
  }

  void begin(uint8_t *mac_address, IPAddress local_ip)                       {}

  void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress dns_server) {}

  void begin(uint8_t  *mac,
             IPAddress ip,
             IPAddress dns,
             IPAddress gateway,
             IPAddress subnet) {}
};

extern EthernetClass Ethernet;
class EthernetUDP : public UDP {
public:

  EthernetUDP() {}
};
class EthernetClient : public Client {
public:

  virtual void setConnectionTimeout(uint16_t timeout) {}
};

#endif /* ETHERNET_H_ */
