#ifndef FMTG
#define FMTG
//library for FMTG protocol
#define ADDR_S 2

typedef struct{
  byte src[3];
  byte dst[3];
  byte is[3];
  byte ir[3];
  byte type;
  uint16_t hop;
  byte payload[16];
} fmtg;

fmtg construct_discovery(byte src[2], byte dst[2], uint16_t hop){
	fmtg packet;
	memcpy(packet.src, src, ADDR_S); 
	memcpy(packet.dst, dst, ADDR_S); 
	memcpy(packet.is, src, ADDR_S); 
	memcpy(packet.ir, "ff", ADDR_S);
       	packet.type = 'D';
	packet.hop = hop;
	packet.payload[0] = '\0';
	return packet;

}

fmtg construct_ack(byte src[], byte dst[], byte iReceiver[])
{
	fmtg packet;
	memcpy(packet.src, src, ADDR_S); 
	memcpy(packet.dst, dst, ADDR_S); 
	memcpy(packet.is, src, ADDR_S); 
	memcpy(packet.ir, iReceiver, ADDR_S);
       	packet.type = 'A';
	packet.payload[0] = '\0';
	return packet;
}

void printp(fmtg packet)
{
	char buff[3];
	char payload[17];
	buff[2] = '\0';
	payload[16] = '\0';
	Serial.print("Src: "); Serial.println((char*)memcpy(buff, packet.src, ADDR_S));
	Serial.print("Dst: ");Serial.println((char*)memcpy(buff, packet.dst, ADDR_S));
	Serial.print("Isnd: ");Serial.println((char*)memcpy(buff, packet.is, ADDR_S));
	Serial.print("Irecv: ");Serial.println((char*)memcpy(buff, packet.ir, ADDR_S));
	Serial.print("Type: ");Serial.println((char)packet.type);
	Serial.print("Hop: ");Serial.println((int)packet.hop);
	Serial.print("Payload: ");Serial.println((char*)memcpy(payload, packet.payload, 16));
}
#endif
