#ifndef UTILS_H
#define UTILS_H

#include "fmtg.h"
#include <RF24.h>

// Function to convert our 2 byte address into the full 5 byte address for NRF Module
uint8_t *full_addr(byte addr[ADDR_S]);

void assign_address(byte address[ADDR_S]);

void broadcast(RF24 *transmitter, RF24 *receiver, fmtg *packet);

void unicast(RF24 *transmitter, fmtg *packet);

void printp(fmtg packet);

int addrcmp(const byte addr1[], const byte addr2[]);

void *addrcpy(byte addr1[], const byte addr2[]);
#endif
