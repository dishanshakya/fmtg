/* FMTG Protocol Library */

#ifndef FMTG
#define FMTG

#include <RF24.h>
#include <SPI.h>
#include "constants.h"

extern const byte addr[ADDR_S];

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


fmtg construct_discovery(byte dst[ADDR_S]);

fmtg construct_reconnect(fmtg packet);

fmtg construct_ack(fmtg* discovery);

fmtg construct_relay_pkt(fmtg *packet);

fmtg construct_data_from_ack(fmtg packet, const byte *buff);

#endif
