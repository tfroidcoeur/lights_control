#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use
                            CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>
#include <time.h>
#include <util/eu_dst.h>
#include <stdio.h>
#include <MemoryFree.h>

#include "Controller.h"
#include "logging.h"
#include "net.h"

static Actor *controller;

#ifdef USE_NET
Net net;
#endif // ifdef USE_NET

#ifdef USE_NTP
# include "NTPClient.h"

/* near daily */
# define NTP_PERIOD (3600 * 24 * 1000 + 3141)

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP udp;

// NTP client sync at regular intervals
// use own openwrt gateway with ntp server by ip
// to minimize delays and skip dns lookup
// because NTPClient will block when querying the
NTPClient ntpclient(udp, "10.0.0.1", 0, NTP_PERIOD, 100);
#endif // ifdef USE_NTP

// #define DEBUG
// the setup function runs once when you press reset (CONTROLLINO RST button) or
// connect the CONTROLLINO to the PC
void setup() {
  Serial.begin(115200);
  COUT_DEBUG(cout << "size of Controller " << sizeof(Controller) << endl);
  COUT_DEBUG(cout << "Lights version " << VERSION << endl);
  COUT_DEBUG(cout << "free: " << freeMemory() << endl; )
  controller = createController();
  controller->setup();
  COUT_DEBUG(cout << "controller setup done" << endl);

#ifdef USE_NET
  net.setup();
#endif // ifdef USE_NET
#ifdef USE_NTP
  ntpclient.begin();

  // set up for Belgium
  set_zone(1 * ONE_HOUR);
  set_dst(eu_dst);
#endif // ifdef USE_NTP

  cout << "free: " << freeMemory() << endl;

  cout << "setup done" << endl;
}

#ifdef DEBUG
void checkTimeSpent(int maxtime, char *where) {
  static unsigned long last = 0;
  unsigned long newt        = millis();

  if ((newt - last > maxtime) && last) {
    cout << (newt - last);
    cout << " spent in " << where << endl;
  }
  last = newt;
}

#else // ifdef DEBUG
# define checkTimeSpent(a, b)
#endif // ifdef DEBUG

// the loop function runs over and over again forever
void loop() {
  checkTimeSpent(50, "start");
#ifdef USE_NET
  net.handle();
  checkTimeSpent(10, "DHCP");
#endif // ifdef USE_NET
#ifdef DEBUG
# ifdef USE_NTP
  time_t t;
  struct tm ts;
  char buf[80];
# endif // ifdef USE_NTP
# ifdef USE_NET
  static const unsigned long REFRESH_INTERVAL = 10000; // ms
  static unsigned long lastRefreshTime        = 0;
# endif // ifdef USE_NET
#endif  // ifdef DEBUG

  controller->handle();
  checkTimeSpent(50, "controller");

#ifdef USE_NTP
  ntpclient.checkSend();
  checkTimeSpent(50, "ntp send");
  ntpclient.checkUpdate();
  checkTimeSpent(50, "ntp receive");
#endif // ifdef USE_NTP

#ifdef DEBUG
# ifdef USE_NET

  if (millis() - lastRefreshTime >= REFRESH_INTERVAL)
  {
    lastRefreshTime = millis();
    cout << "ip: ";
    Serial.print(Ethernet.localIP());
    cout << "/";
    Serial.print(Ethernet.subnetMask());
    cout << endl;
    cout << "gw: ";
    Serial.print(Ethernet.gatewayIP());
    cout << endl;
    cout << "dns: ";
    Serial.print(Ethernet.dnsServerIP());
    cout << endl;
#  ifdef USE_NTP
    Serial.println(ntpclient.getFormattedTime());
    t = ntpclient.getEpochTime() - UNIX_OFFSET;
    Serial.println(ntpclient.getEpochTime());
    Serial.println(        t);


    // not really needed, unless we want to use time() to obtain time
    // set_system_time(t);

    localtime_r(&t, &ts);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    cout << buf << endl;

    gmtime_r(&t, &ts);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    cout << buf << endl;

    memset(&ts, 0, sizeof(ts));
    ts.tm_year = 118;
    ts.tm_mon  = 3;
    ts.tm_mday = 4;
    t          = mktime(&ts);
    Serial.println(t);

    gmtime_r(&t, &ts);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    cout << buf << endl;
#  endif // ifdef USE_NTP
# endif  // ifdef USE_NET
}

#endif   // ifdef DEBUG
  checkTimeSpent(50, "debug prints");
}
