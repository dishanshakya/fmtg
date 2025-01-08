#include "routing.h"

RoutingEntry routing_table[MAX_ENTRIES];

void initRoutingTable() {
    memset(routing_table, 0, sizeof(routing_table));
}

int isEmptyEntry(const RoutingEntry entry) {
    RoutingEntry empty;
    memset(&empty, 0, sizeof(RoutingEntry));
    return memcmp(&entry, &empty, sizeof(RoutingEntry)) == 0;
}

RoutingEntry createEntryFromPacket(fmtg *packet){
	RoutingEntry e;
	memcpy(e.src, packet->src, ADDR_S);
	memcpy(e.dst, packet->dst, ADDR_S);
	memcpy(e.ir, packet->ir, ADDR_S);
	memcpy(e.is, packet->is, ADDR_S);
	return e;
}

int duplicate(fmtg *packet){
    RoutingEntry entry = createEntryFromPacket(packet);
    for(int i = 0; i < MAX_ENTRIES; i++){
        if(!memcmp(&routing_table[i], &entry, sizeof(RoutingEntry))){
            Serial.println("Duplicate");
            return 1;
        }
    }

    return 0;
}

void relay(RF24 *transmitter, RF24 *receiver, fmtg *packet){
    fmtg relay_packet = construct_relay_pkt(packet);
    for(int i = 0; i < MAX_ENTRIES; i++){
        if (!memcmp(packet->src, routing_table[i].src, ADDR_S) && 
            !memcmp(packet->dst, routing_table[i].dst, ADDR_S)) {
            memcpy(relay_packet.ir, routing_table[i].ir, ADDR_S);
            unicast(transmitter, &relay_packet);
        } else if (!memcmp(packet->src, routing_table[i].dst, ADDR_S) && 
                !memcmp(packet->dst, routing_table[i].src, ADDR_S)) {
            memcpy(relay_packet.ir, routing_table[i].ir, ADDR_S);
            unicast(transmitter, &relay_packet);
        } else {
            memcpy(relay_packet.ir, BROADCAST_ADDR, ADDR_S);
            broadcast(transmitter, receiver, &relay_packet);
        }

    }
}

void insertEntry(fmtg *packet) {
    RoutingEntry entry = createEntryFromPacket(packet);
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (isEmptyEntry(routing_table[i]) && !duplicate(packet)) {
            memcpy(&routing_table[i], &entry, sizeof(RoutingEntry));
            displayTable();
            return;
        }
    }
    Serial.println("full");
    displayTable();
}
void displayTable() {
    Serial.println("\nRouting Table:");
    Serial.println("---------------------------------");
    Serial.println("| i | src | dst | ir | is |");
    Serial.println("---------------------------------");

    char buffer[ADDR_S + 1]; 
    buffer[ADDR_S] = '\0'; 

    for (int i = 0; i < MAX_ENTRIES; i++) {
        Serial.print(" | ");
        Serial.print(i);
        Serial.print(" |");

        if (!isEmptyEntry(routing_table[i])) {
            Serial.print((const char *)memcpy(buffer, routing_table[i].src, ADDR_S));
            Serial.print(" | ");

            Serial.print((const char *)memcpy(buffer, routing_table[i].dst, ADDR_S));
            Serial.print(" | ");

            Serial.print((const char *)memcpy(buffer, routing_table[i].ir, ADDR_S));
            Serial.print(" | ");

            Serial.print((const char *)memcpy(buffer, routing_table[i].is, ADDR_S));
            Serial.println(" |");
        } else {
            Serial.println("null |null |null |null |");
        }
    }
    Serial.println("----------------------------------------------------------------");
}