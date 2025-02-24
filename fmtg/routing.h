#ifndef ROUTING_H
#define ROUTING_H

#include "constants.h"
#include "fmtg.h"
#include "utils.h"

struct RoutingEntry{
    uint16_t src, dst, ir, is;
    uint8_t irpc, ispc, irchannel, ischannel;

    RoutingEntry(uint16_t src, uint16_t dst, uint16_t ir, uint16_t is, uint8_t irchannel, uint8_t ischannel);
    RoutingEntry(){}
    RoutingEntry(fmtg *packet);
};

extern RoutingEntry routing_table[MAX_ENTRIES];

extern RoutingEntry *ownEntry;

void initRoutingTable();

int isEmptyEntry(const RoutingEntry entry);

int duplicate(fmtg *packet);

void insertEntry(fmtg *packet);

void deleteEntry(int index);

void displayTable();

int search(fmtg *packet);

#endif
