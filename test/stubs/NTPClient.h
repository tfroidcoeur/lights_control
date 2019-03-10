/*
 * NTPClient.h
 *
 *  Created on: Mar 10, 2019
 *      Author: fraco
 */
#include "Udp.h"

#ifndef NTPCLIENT_H_
#define NTPCLIENT_H_
typedef const char * String;
class NTPClient {
  public:
    NTPClient(UDP& udp);
    NTPClient(UDP& udp, long timeOffset);
    NTPClient(UDP& udp, const char* poolServerName);
    NTPClient(UDP& udp, const char* poolServerName, long timeOffset);
    NTPClient(UDP& udp, const char* poolServerName, long timeOffset, unsigned long updateInterval) {};
    void begin() {};

//    void begin(int port);

//    bool update();

//    bool forceUpdate();

    bool checkUpdate(){return true;};
    bool checkSend(){return true;};
    String getFormattedTime() const { return "test string";};
};



#endif /* NTPCLIENT_H_ */
