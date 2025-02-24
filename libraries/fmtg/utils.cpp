#include "utils.h"
#include <TimerOne.h>

uint8_t *full_addr(byte addr[ADDR_S]){
        static byte address[FULL_ADDR_S+1];
        memset(address, '0', FULL_ADDR_S);
        memcpy(address, addr, ADDR_S);
        address[FULL_ADDR_S] = '\0';
        return address;
}

void assign_address(byte address[ADDR_S]){
	memcpy(addr, address, ADDR_S);
}

int addrcmp(const byte addr1[ADDR_S], const byte addr2[ADDR_S]){
    return !memcmp(addr1, addr2, ADDR_S);
}

void *addrcpy(byte addr1[ADDR_S], const byte addr2[ADDR_S]){
    return memcpy(addr1, addr2, ADDR_S);
}

void broadcast(RF24 *transmitter, RF24 *receiver, fmtg *packet){
    byte broadcast_addr[5];
    memcpy(broadcast_addr, full_addr(BROADCAST_ADDR), FULL_ADDR_S);
    transmitter->openWritingPipe(broadcast_addr);
    receiver->closeReadingPipe(1);
    transmitter->write(packet, sizeof(fmtg));
    byte broadcast_pipe[5];
    memcpy(broadcast_pipe, full_addr(BROADCAST_ADDR), FULL_ADDR_S);
}

void unicast(RF24 *transmitter, fmtg *packet){
    byte unicast_addr[5];
    memcpy(unicast_addr, full_addr(packet->ir), FULL_ADDR_S);
    transmitter->openWritingPipe(unicast_addr);
    transmitter->write(packet, sizeof(fmtg));
}

void printp(fmtg packet)
{
        char buff[ADDR_S+1];
        char payload[PAYLOAD_S+1];
        buff[ADDR_S] = '\0';
        payload[PAYLOAD_S] = '\0';
        Serial.print("Src: ");          Serial.println((char*)memcpy(buff, packet.src, ADDR_S));
        Serial.print("Dst: ");          Serial.println((char*)memcpy(buff, packet.dst, ADDR_S));
        Serial.print("Isnd: ");         Serial.println((char*)memcpy(buff, packet.is, ADDR_S));
        Serial.print("Irecv: ");        Serial.println((char*)memcpy(buff, packet.ir, ADDR_S));
        Serial.print("Type: ");         Serial.println((char)packet.type);
        Serial.print("Hop: ");          Serial.println((int)packet.hop);
        Serial.print("Payload: ");      Serial.println((char*)memcpy(payload, packet.payload, PAYLOAD_S));
}
