#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>
#include "Controller.h"
#include "Ethernet.h"
#include "NTPClient.h"
#include <time.h>
#include <util/eu_dst.h>
#include <stdio.h>

static Controller controller;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xA8, 0x61, 0x0A, 0x01, 0xFE, 0xED
};
#ifdef FROIDCOEUR_GW
IPAddress ip(10, 0, 0, 230);
IPAddress gw(10, 0, 0, 1);
IPAddress dns(10, 0, 0, 1);
IPAddress netmask(255, 255, 255, 0);
#else
IPAddress ip(192, 168, 0, 57);
IPAddress gw(192, 168, 0, 1);
IPAddress dns(8, 8, 8, 8);
IPAddress netmask(255, 255, 255, 0);
#endif

/* near daily */
#define NTP_PERIOD (3600*24*1000+3141)

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP udp;

// NTP client sync at regular intervals
NTPClient ntpclient(udp, "pool.ntp.org", 0, NTP_PERIOD);

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
void setup() {
	Serial.begin(9600);
	controller.setup();

	Ethernet.begin(mac, ip, dns, gw, netmask);
	ntpclient.begin();

	// set up for Belgium
	set_zone(1*ONE_HOUR);
	set_dst(eu_dst);

	Serial.println("setup done");
}

// the loop function runs over and over again forever
void loop() {
	time_t t;
	struct tm ts;
	char buf[80];
	static const unsigned long REFRESH_INTERVAL = 10000; // ms
	static unsigned long lastRefreshTime = 0;

	controller.handle();
	ntpclient.checkSend();
	ntpclient.checkUpdate();

	if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
	{
		lastRefreshTime = millis();
		Serial.println(ntpclient.getFormattedTime());
		t=ntpclient.getEpochTime()-UNIX_OFFSET;
		// not really needed, unless we want to use time() to obtain time
		// set_system_time(t);

		localtime_r(&t,&ts);
		strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
		cout << buf << endl;
	}
}
