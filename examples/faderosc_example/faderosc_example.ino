#include <SPI.h>
#include <Ethernet.h>
#include <SLIPEncodedSerial.h>

/*
 * Configure the osc addresses as C string constant here.
 * The master fader ist always bound to the analog pin 0.
 * The following 5 analog pins will get the osc address suffix 0 to 4.
 *
 * If OSC_ADDRESS_MASTER is undefined, the master fader is not used
 * and all osc suffixes are mapped to the corresponding analog pin number.
 *
 * for example:
 * #define OSC_ADDRESS "/fader/"
 * #define OSC_ADDRESS_MASTER OSC_ADDRESS "master"
 *
 * A0 -> "/fader/master"
 * A1 -> "/fader/0"
 * A2 -> "/fader/1"
 * A3 -> "/fader/2"
 *
 * ATTENTION: alwasy define these 2 macros BEFORE including FaderOSC.h! Otherwise the defines are ignored.
 */
#define OSC_ADDRESS         "/net/mhcloud/volume/jack-volume/"
#define OSC_ADDRESS_MASTER   OSC_ADDRESS "master"
#include <FaderOSC.h>

// MAC address of the ethernet interface
static byte mac[] = { 0x90, 0xa2, 0xda, 0x0e, 0x0d, 0xb9 };
// IP address of your osc server you want to connect to
static IPAddress remoteip(192,168,0,42);
// local UDP port. It does not matter so pick a random number between 1 and 65535
static const unsigned int localport = 7600;
// UDP port of your osc server. The default of jack-volume is 7600
static const unsigned int remoteport = 7600;

/*
 * Create a FaderOSC object with a positive number of used potentiometers/faders/analog pins
 * as template argument.
 */
static FaderOSC<1> faderosc;
// global EthernetUDP object
static EthernetUDP udp;
// global object for transmitting SLIP packets over the serial interface
static SLIPEncodedSerial SLIPSerial(Serial);

void setup() {
  // start ethernet using dhcp
  Ethernet.begin(mac);
  // start the udp server and client
  udp.begin(localport);
  
  // initialize the FaderOSC object with the osc server ip address
  faderosc.begin(remoteip);
  
  /*
   * The following lines configure the communication interfaces FaderOSC should use.
   * These two calls are optional but can also be used together.
   */
  // tell FaderOSC to send osc packets via udp
  faderosc.useUDP(&udp, remoteport);
  // send osc packets wrapped in SLIP packets over a serial interface
  faderosc.useSerial(&SLIPSerial);
}

void loop() {
  // perform one iteration of polling the analog inputs and sending osc messages
  faderosc.iteration();
}
