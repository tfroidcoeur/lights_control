#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <HardwareSerial.h>
#include "Controller.h"
#include "Ethernet.h"
#include "NTPClient.h"

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

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP udp;

// NTP client sync at regular intervals
NTPClient ntpclient(udp, "pool.ntp.org", 0, 60000);

// the setup function runs once when you press reset (CONTROLLINO RST button) or connect the CONTROLLINO to the PC
void setup() {
	Serial.begin(9600);
	controller.setup();

	Ethernet.begin(mac, ip, dns, gw, netmask);
	ntpclient.begin();

	Serial.println("setup done");
}

// the loop function runs over and over again forever
void loop() {
	static const unsigned long REFRESH_INTERVAL = 5000; // ms
	static unsigned long lastRefreshTime = 0;

	controller.handle();
	ntpclient.checkSend();
	ntpclient.checkUpdate();

	if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
	{
		lastRefreshTime += REFRESH_INTERVAL;
		Serial.println(ntpclient.getFormattedTime());
	}
}
