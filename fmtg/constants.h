#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

#define ADDR_S 2
#define FULL_ADDR_S 5
#define PAYLOAD_S 21
#define MAX_ENTRIES 4

const uint16_t BROADCAST_ADDR = 0xffff;

const byte P_DISC = 'D';
const byte P_ACK = 'A';
const byte P_DAT = 'd';
const byte P_REC = 'R';
const byte P_TERM = 'T';

const int BAUD_RATE = 115200;

#endif
