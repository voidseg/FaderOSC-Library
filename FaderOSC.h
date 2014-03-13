/*
 * FaderOSC.h - Library for using potentiometers with OSC (Open Sound Control)
 * Make sure to install the oscuino library (http://cnmat.berkeley.edu/oscuino)
 * because we use it for packing the OSC messages.
 * 
 * Created by Michael Hoefler, March 12, 2014.
 * For bug reports and feature requests please email me at voidseg@gmail.com
 */

#ifndef FADEROSC_H
#define FADEROSC_H

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SLIPEncodedSerial.h>
#include <OSCMessage.h>

/*
 * Define OSC_ADDRESS and OSC_ADDRESS_MASTER to configure the OSC adresses.
 * This MUST be done BEFORE including this file into your sketch!
 */
#ifndef OSC_ADDRESS
#define OSC_ADDRESS "/faderosc/fader/"
#endif
#if 0
#ifndef OSC_ADDRESS_MASTER
#define OSC_ADDRESS_MASTER OSC_ADDRESS "master"
#endif
#endif

/*
 * This class can be used for transmitting analog values from an Arduino board
 * over UDP oder serial interface as OSC messages. This requires either a serial
 * interface or an ethernet port or both. A variable number of analog pins can
 * be used by this library. This number is configured by the template argument
 * "NUM_POTS".
 * 
 * The library is called "FaderOSC" because it is ment for using it with analog
 * faders as input device. Other potentiometers or sensors connected to the analog
 * pins will also work.
 * 
 * The addresses of all OSC messages contain a fixed prefix, a fader specific
 * suffix and a float value between 0.0 and 1.0 (including) representing the
 * analog value. E. g.:
 * |  prefix   | suffix | value |
 * "/faderosc/"   "2"      0.42
 * 
 * This library was built for controlling a jack-volume instance but can be used
 * for multiple purposes too.
 */
template<uint8_t NUM_POTS>
class FaderOSC {
public:
	FaderOSC()
	: messages(),
	  old_vals(),
	  udp(NULL),
	  tcp(NULL),
	  ser(NULL),
	  server({192, 168, 0, 42}),
	  port(7600)
	{
	}
	~FaderOSC() {
	}
	
	/*
	 * Tell FaderOSC to send UDP packets. This function can be called in
	 * setup() after begin().
	 * 
	 * \param	udp		A pointer to the global UDP object.
	 * \param	port	The destination port for UDP packets.
	 */
	void useUDP(EthernetUDP* udp, unsigned int port) {
        this->udp = udp;
		this->port = port;
	}
	
#if 0
	void useTCP(EthernetClass* tcp) {
		this->tcp = tcp;
	}
#endif

	/*
	 * Tell FaderOSC to send SLIP packets over a serial interface. This
	 * function can be called in setup() after begin().
	 * 
	 * \param	ser		A pointer to the global SLIPEncodedSerial object.
	 */
	void useSerial(SLIPEncodedSerial* ser) {
		this->ser = ser;
	}
	
	/*
	 * Initialize FaderOSC object with the OSC server ip address.
	 * This function MUST be called in setup().
	 * 
	 * \param	server	The IP address of the OSC server.
	 */
	void begin(IPAddress& server) {
		this->server = server;
		
		int suff = 0;
#ifdef OSC_ADDRESS_MASTER
		int i = 1;
#else
		int i = 0;
#endif
		
		while (i<NUM_POTS) {
			char address[] = OSC_ADDRESS "   ";
			int base_len = strlen(OSC_ADDRESS);
			address[base_len + 0] = ' ';
			address[base_len + 1] = ' ';
			address[base_len + 2] = ' ';
			itoa(suff, address + base_len, 10);
			messages[i].setAddress(address);
			suff++;
			i++;
		}
#ifdef OSC_ADDRESS_MASTER
		messages[0].setAddress(OSC_ADDRESS_MASTER);
#endif
	}
	
	/*
	 * Call this function once in every loop iteration. It will read the analog
	 * inputs end sends all OSC messages.
	 */
	int iteration() {
		int err = 0;
	
		for (int i=0; i<NUM_POTS; i++) {
			int val = analogRead(i);
			//TODO: calc average val from previous values
			if (abs(val - old_vals[i]) > 5) {
				messages[i].empty();
				old_vals[i] = val;
				float f = val / (float)1023.0;
				messages[i].add(f);
				if (messages[i].hasError()) {
					err = -1;
					continue;
				}
				if (udp != NULL) {
					udp->beginPacket(server, port);
					messages[i].send(*udp);
					if (messages[i].hasError()) {
						err = -1;
						udp->write(' ');
					}
					udp->endPacket();
				}
				if (ser != NULL) {
					messages[i].send(*ser);
					ser->endPacket();
				}
			}
		}
		return err;
	}
	
	void end() {
	}

private:
	OSCMessage messages[NUM_POTS];
	int old_vals[NUM_POTS];
	EthernetUDP* udp;
	EthernetClass* tcp;
	SLIPEncodedSerial* ser;
	IPAddress server;
	uint16_t port;
};

#endif /* FADEROSC_H */