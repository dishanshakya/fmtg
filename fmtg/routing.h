#ifndef ROUTING_H
#define ROUTING_H

#include "constants.h"
#include "fmtg.h"
#include "utils.h"


typedef struct {
    byte src[ADDR_S];
    byte dst[ADDR_S];
    byte ir[ADDR_S];
    byte is[ADDR_S];
} RoutingEntry;

extern RoutingEntry routing_table[MAX_ENTRIES];

void initRoutingTable();

int isEmptyEntry(const RoutingEntry entry);

RoutingEntry createEntryFromPacket(fmtg *packet);

int duplicate(fmtg *packet);

void relay(RF24 *transmitter, RF24 *receiver, fmtg *packet);

void insertEntry(fmtg *packet);

void displayTable();

int search(fmtg *packet);

#endif
