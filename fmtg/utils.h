#ifndef UTILS_H
#define UTILS_H

#include "fmtg.h"
#include <RF24.h>

void initRadio(RF24* radio);

void assign_address(uint16_t address);

void broadcast(RF24 *transmitter, RF24 *receiver, fmtg *packet);

void unicast(RF24 *transmitter, fmtg *packet);

void printp(fmtg *packet);

void callAfterSeconds(void (*func)()); 

void repeat(void (*func)(), unsigned long intervalMicros);

void repeat100ms(void (*func)());

void stop100ms();
#endif
