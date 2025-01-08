#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Arduino.h>

#define ADDR_S 2
#define FULL_ADDR_S 5
#define PAYLOAD_S 21
#define MAX_ENTRIES 4

const byte BROADCAST_ADDR[ADDR_S] = "ff";
const byte P_DISC = 'D';
const byte P_ACK = 'A';
const byte P_DAT = 'd';
const int BAUD_RATE = 115200;

#endif
