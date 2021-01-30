#include <Controllino.h>  /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
#include <Ethernet.h>
#include <utility/w5100.h>
#include "net.h"
#include "logging.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xA8, 0x61, 0x0A, 0x01, 0xFE, 0xEC
};
#ifdef FROIDCOEUR_GW
IPAddress ip(10, 0, 0, 231);
IPAddress gw(10, 0, 0, 1);
IPAddress dns(10, 0, 0, 1);
IPAddress netmask(255, 255, 255, 0);
#else
IPAddress ip(192, 168, 0, 57);
IPAddress gw(192, 168, 0, 1);
IPAddress dns(8, 8, 8, 8);
IPAddress netmask(255, 255, 255, 0);
#endif
void Net::setup() {
	Ethernet.begin(mac, ip, dns, gw, netmask);
	// Ethernet.begin(mac);
	// crazy chip: 500 = 50ms
	// and w the ##$@$ do you wait for UDP to succeed
	// TODO make sure the crazy chip does send the UDP packet,
	// check what normal times are for this process
	W5100.setRetransmissionTime(500);
	W5100.setRetransmissionCount(1);
	COUT_DEBUG(cout << "net setup done" << endl);
}

void Net::handle() {
	Ethernet.maintain();
}
