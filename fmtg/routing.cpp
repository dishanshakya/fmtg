#include "routing.h"

RoutingEntry::RoutingEntry(uint16_t src, uint16_t dst, uint16_t ir, uint16_t is, uint8_t irchannel, uint8_t ischannel)
{
	this->src = src;
	this->dst = dst;
	this->is = is;
	this->ir = ir;
	this->ispc = 0;
	this->irpc = 0;
	this->irchannel = irchannel;
	this->ischannel = ischannel;
}

RoutingEntry::RoutingEntry(fmtg *packet){
	this->src = packet->src;
	this->dst = packet->dst;
	this->ir = packet->ir;
	this->is = packet->is;
	this->ispc = 0;
	this->irpc = 0;
	this->ischannel = packet->ischannel;
	this->irchannel = packet->irchannel;
}

RoutingEntry routing_table[MAX_ENTRIES];

RoutingEntry *ownEntry = routing_table;

void initRoutingTable() {
    memset(routing_table, 0, sizeof(routing_table));
}

int isEmptyEntry(const RoutingEntry entry) {
    RoutingEntry empty;
    memset(&empty, 0, sizeof(RoutingEntry));
    return memcmp(&entry, &empty, sizeof(RoutingEntry)) == 0;
}

int duplicate(fmtg *packet){
    for(int i = 0; i < MAX_ENTRIES; i++){
        if(packet->src == routing_table[i].src && packet->dst == routing_table[i].dst || packet->dst == routing_table[i].src && packet->src == routing_table[i].dst){
            Serial.println("Duplicate");
            return 1;
        }
    }

    return 0;
}


int search(fmtg *packet){
    for(int i = 1; i < MAX_ENTRIES; i++){
        if(packet->src == routing_table[i].src && packet->dst == routing_table[i].dst ||
        packet->src == routing_table[i].dst && packet->dst == routing_table[i].src){
            return i;
        }
    }
    return -1;
}

void insertEntry(fmtg *packet) {
    RoutingEntry entry(packet);
	if(duplicate(packet)) return;
    for (int i = 1; i < MAX_ENTRIES; i++) {
        if (isEmptyEntry(routing_table[i])) {
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
    Serial.println("| i | src | dst | ir | is | ispc | irpc |");
    Serial.println("---------------------------------");

    char buffer[ADDR_S + 1]; 
    buffer[ADDR_S] = '\0'; 

    for (int i = 0; i < MAX_ENTRIES; i++) {
        Serial.print(" | ");
        Serial.print(i);
        Serial.print(" |");

        if (!isEmptyEntry(routing_table[i])) {
            Serial.print(routing_table[i].src, HEX);
            Serial.print(" | ");

            Serial.print(routing_table[i].dst, HEX);
            Serial.print(" | ");

            Serial.print(routing_table[i].ir, HEX);
            Serial.print(" | ");

            Serial.print(routing_table[i].is, HEX);
            Serial.print(" |");

	    Serial.print(routing_table[i].ispc);
	    Serial.print(" |");

	    Serial.print(routing_table[i].irpc);
	    Serial.println(" |");
        } else {
            Serial.println("null |null |null |null |null |null |");
        }
    }
    Serial.println("----------------------------------------------------------------");
}
