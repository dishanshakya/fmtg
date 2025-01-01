#include <RF24.h>
#include <SPI.h>
#include <printf.h>
#include <stdint.h>
#include <fmtg.h>

byte broadcast[5] =  "ff000";
byte addr[5] = "11000";

RF24 transmitter(4,5);
RF24 receiver(2,3);


void handleReceiver(fmtg* packet){

}

void handleRecepient(){}

void nonReceiver(){}

void listen(){
  fmtg *packet = malloc(sizeof(fmtg));
  if(receiver.available()){
    receiver.read(packet, sizeof(fmtg));
  }

}

void setup() {
  Serial.begin(115200);
  printf_begin();

  transmitter.begin();
  transmitter.setDataRate(1);
  transmitter.setCRCLength(2);
  transmitter.stopListening();
  


  receiver.begin();
  receiver.setDataRate(1);
  receiver.setCRCLength(2);
  receiver.openReadingPipe(0, addr);
  receiver.openReadingPipe(1, broadcast);
  receiver.startListening();

  transmitter.openWritingPipe(broadcast);

}

void loop() {
  fmtg packet, trash;
  byte iReceiver[5] = "00000";
  if(receiver.available()){
    delay(1000);
    Serial.println("===============av==============");
    receiver.read(&packet, sizeof(fmtg));
    receiver.flush_rx();
    fmtg ack = construct_ack(addr, packet.src, packet.is);
    memcpy(iReceiver, packet.is, ADDR_S);
    Serial.println((char*)iReceiver);
    transmitter.openWritingPipe(iReceiver);
    transmitter.write(&ack, sizeof(fmtg));
    printp(packet);
    // if(strcmp(packet.src, "11") == 0);
    // else if(strcmp(packet.ir, "ff") == 0){
    //   Serial.println("Rebroadcasting...");
    //   fmtg rebroadcast = construct_discovery(broadcast, ++packet.hop);
    //   transmitter.write(&rebroadcast, sizeof(rebroadcast));
    //   receiver.read(&trash, sizeof(trash));
    //   transmitter.write(&rebroadcast, sizeof(rebroadcast));
    //   receiver.read(&trash, sizeof(packet));
    // }
  }
}
