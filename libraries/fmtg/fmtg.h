/* FMTG Protocol Library */

#ifndef FMTG
#define FMTG

#include "ecc.h"
#include "constants.h"

#define ADDR_S 2

const byte BROADCAST_ADDR[2] = "ff";

// An FMTG packet structure
typedef struct{
  byte src[2];		// Source Address
  byte dst[2];		// Destination Address
  byte is[2];		// Intermediate Sender
  byte ir[2];		// Intermediate Receiver
  byte type;		// Type of packet (P_DISC, P_ACK, P_DAT)
  uint16_t hop;		// Hop count
  byte payload[21];
} fmtg;

fmtg construct_discovery(byte dst[2], uint16_t hop){
	fmtg packet;
	memcpy(packet.src, addr, ADDR_S); 
	memcpy(packet.dst, dst, ADDR_S); 
	memcpy(packet.is, addr, ADDR_S); 
	memcpy(packet.ir, BROADCAST_ADDR, ADDR_S);
       	packet.type = P_DISC;
	packet.hop = 0;
	packet.payload[0] = '\0';
	return packet;
}

fmtg construct_ack(fmtg* discovery)
{
	fmtg packet;
	memcpy(packet.src, discovery.dst, ADDR_S); 
	memcpy(packet.dst, discovery.src, ADDR_S); 
	memcpy(packet.is, addr, ADDR_S); 
	memcpy(packet.ir, discovery.is, ADDR_S);
       	packet.type = 'A';
	packet.payload[0] = '\0';
	return packet;
}

void printp(fmtg packet)
{
	char buff[3];
	char payload[17];
	buff[2] = '\0';
	payload[16] = '\0';
	Serial.print("Src: "); Serial.println((char*)memcpy(buff, packet.src, ADDR_S));
	Serial.print("Dst: ");Serial.println((char*)memcpy(buff, packet.dst, ADDR_S));
	Serial.print("Isnd: ");Serial.println((char*)memcpy(buff, packet.is, ADDR_S));
	Serial.print("Irecv: ");Serial.println((char*)memcpy(buff, packet.ir, ADDR_S));
	Serial.print("Type: ");Serial.println((char)packet.type);
	Serial.print("Hop: ");Serial.println((int)packet.hop);
	Serial.print("Payload: ");Serial.println((char*)memcpy(payload, packet.payload, 16));
}
#endif
