/* FMTG Protocol Library */

#ifndef FMTG
#define FMTG

#include <RF24.h>
#include <SPI.h>
#include "ecc.h"

const byte addr[ADDR_S];

// An FMTG packet structure
typedef struct{
    byte src[ADDR_S];		// Source Address
    byte dst[ADDR_S];		// Destination Address
    byte is[ADDR_S];		// Intermediate Sender
    byte ir[ADDR_S];		// Intermediate Receiver
    byte type;			// Type of packet (P_DISC, P_ACK, P_DAT)
    uint16_t hop;			// Hop count
    byte payload[PAYLOAD_S];
} fmtg;


fmtg construct_discovery(byte dst[ADDR_S]){
	fmtg packet;
	memcpy(packet.src, addr, ADDR_S); 
	memcpy(packet.dst, dst, ADDR_S); 
	memcpy(packet.is, addr, ADDR_S); 
	memcpy(packet.ir, BROADCAST_ADDR, ADDR_S);
    packet.type = P_DISC;
	packet.hop = 0;
	memset(packet.payload, 0x00, PAYLOAD_S);
	return packet;
}

fmtg construct_ack(fmtg* discovery)
{
	fmtg packet;
	memcpy(packet.src, discovery->dst, ADDR_S); 
	memcpy(packet.dst, discovery->src, ADDR_S); 
	memcpy(packet.is, addr, ADDR_S); 
	memcpy(packet.ir, discovery->is, ADDR_S);
    packet.type = P_ACK;
    packet.hop = 0;
	memset(packet.payload, 0x00, PAYLOAD_S);
	return packet;
}



#endif
