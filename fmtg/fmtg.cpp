#include "fmtg.h"

const byte addr[ADDR_S];

fmtg construct_discovery(byte dst[ADDR_S]){
	fmtg packet;
	memcpy(packet.src, addr, ADDR_S); 
	memcpy(packet.dst, dst, ADDR_S); 
	memcpy(packet.is, addr, ADDR_S); 
	memcpy(packet.ir, BROADCAST_ADDR, ADDR_S);
    packet.type = P_DISC;
	packet.hop = 0;
	memset(packet.payload, 0x00, PAYLOAD_S);
	return packet;
}

fmtg construct_reconnect(fmtg packet){
	memcpy(packet.is, addr, ADDR_S); 
	memcpy(packet.ir, BROADCAST_ADDR, ADDR_S);
    packet.type = P_REC;
	packet.hop = 0;
	memset(packet.payload, 0x00, PAYLOAD_S);
	return packet;
}

fmtg construct_ack(fmtg* discovery)
{
	fmtg packet;
	memcpy(packet.src, discovery->dst, ADDR_S); 
	memcpy(packet.dst, discovery->src, ADDR_S); 
	memcpy(packet.is, addr, ADDR_S); 
	memcpy(packet.ir, discovery->is, ADDR_S);
    packet.type = P_ACK;
    packet.hop = 0;
	memset(packet.payload, 0x00, PAYLOAD_S);
	return packet;
}

fmtg construct_relay_pkt(fmtg *packet){
    fmtg relay_packet;
    memcpy(relay_packet.src, packet->src, ADDR_S);
    memcpy(relay_packet.dst, packet->dst, ADDR_S);
    memcpy(relay_packet.is, packet->ir, ADDR_S);
    memcpy(relay_packet.type, packet->type, 1);
    memcpy(relay_packet.payload, packet->payload, PAYLOAD_S);
    relay_packet.hop = ++packet->hop;
    return relay_packet;
}
