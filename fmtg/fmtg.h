/* FMTG Protocol Library */

#ifndef FMTG
#define FMTG

#include <RF24.h>
#include "constants.h"

extern uint16_t addr;

typedef enum {
	IDLE,
	CONNECTING,
	RINGING,
	INCALL,
	RELAY,
	TERMINATED
} state_t;

// An FMTG packet structure
struct fmtg{
    uint16_t src;		// Source Address
    uint16_t dst;		// Destination Address
    uint16_t is;		// Intermediate Sender
    uint16_t ir;		// Intermediate Receiver
    byte type;			// Type of packet (P_DISC, P_ACK, P_DAT)
	uint8_t ischannel, irchannel;
    uint16_t hop;			// Hop count
    byte payload[PAYLOAD_S];

    fmtg(uint16_t src, uint16_t dst, uint16_t is, uint16_t ir, byte type, uint8_t ischannel, uint8_t irchannel);
    fmtg(){}
    void attachPayload(byte type, byte buff[], int n);
};

#endif
