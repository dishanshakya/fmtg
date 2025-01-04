#include "constants.h"
#include "fmtg.h"

typedef struct {
    byte src[ADDR_S];
    byte dst[ADDR_S];
    byte ir[ADDR_S];
    byte is[ADDR_S];
} RoutingEntry;

RoutingEntry* routing_table[MAX_ENTRIES] = {NULL}; 


RoutingEntry createEntryFromPacket(fmtg *packet){
	RoutingEntry e;
	memcpy(e.src, packet->src, ADDR_S);
	memcpy(e.dst, packet->dst, ADDR_S);
	memcpy(e.ir, packet->ir, ADDR_S);
	memcpy(e.is, packet->is, ADDR_S);
	return e;
}


void insertEntry(RoutingEntry* entry) {
    for (int i = 0; i < MAX_ENTRIES; i++) {
        if (routing_table[i] == NULL) {
            routing_table[i] = entry;
            return;
        }
    }
    Serial.println("full");
}


void displayTable() {
    Serial.println("\nRouting Table:");
    Serial.println("----------------------------------------------------------------");
    Serial.println("| i | src | dst | ir | is |");
    Serial.println("----------------------------------------------------------------");

    char buffer[ADDR_S + 1]; 
    buffer[ADDR_S] = '\0'; 

    for (int i = 0; i < MAX_ENTRIES; i++) {
        Serial.print(" | ");
        Serial.print(i);
        Serial.print(" |");

        if (routing_table[i] != NULL) {
            Serial.print((const char *)memcpy(buffer, routing_table[i]->src, ADDR_S));
            Serial.print(" | ");

            Serial.print((const char *)memcpy(buffer, routing_table[i]->dst, ADDR_S));
            Serial.print(" | ");

            Serial.print((const char *)memcpy(buffer, routing_table[i]->ir, ADDR_S));
            Serial.print(" | ");

            Serial.print((const char *)memcpy(buffer, routing_table[i]->is, ADDR_S));
            Serial.println(" |");
        } else {
            Serial.println("null |null |null |null |");
        }
    }
    Serial.println("----------------------------------------------------------------");
}

