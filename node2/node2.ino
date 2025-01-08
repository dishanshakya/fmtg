#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <fmtg.h>
#include <utils.h>
#include <routing.h>
#include <ecc.h>

RF24 transmitter(2,3);
RF24 receiver(4,5); // new bootloader arduino

void handleRecipient(fmtg *packet){
  if(packet->type == P_DISC){
    Serial.println("received a discovery, sending ack");
    fmtg ack = construct_ack(packet);
    unicast(&transmitter, &ack);
  } else if(packet->type == P_ACK){
    Serial.println("Received ack, found recepient!");
  }
  // switch(packet->type){
  //   case P_DISC:
  //   Serial.println("received a discovery, sending ack");
  //   fmtg ack = construct_ack(packet);
  //   printp(ack);
  //   unicast(&transmitter, &ack);
  //   break;

  //   case 65:
  //   Serial.println("Found recepient!");
  //   break;

  //   case P_DAT:
  //   break;

  //   default:
  //   Serial.println("wtf?");
  //   break;
  // }
}

void setup() {

  assign_address(addr_n2); 
  
  initRoutingTable();
  
  transmitter.begin();
  receiver.begin();
  Serial.begin(115200);
  printf_begin();

  // 2mbps data rate
  transmitter.setDataRate(1);
  receiver.setDataRate(1);
  
  transmitter.setCRCLength(2);
  transmitter.setPALevel(3);

  byte unicast_pipe[5];
  byte broadcast_pipe[5];
  memcpy(unicast_pipe, full_addr(addr), FULL_ADDR_S);
  memcpy(broadcast_pipe, full_addr(BROADCAST_ADDR), FULL_ADDR_S);
  receiver.openReadingPipe(0, unicast_pipe);
  receiver.openReadingPipe(1, broadcast_pipe);
  receiver.startListening();
  transmitter.stopListening();

  // transmitter.printDetails();
  // receiver.printDetails();  

  fmtg discovery = construct_discovery(addr_n3);
  broadcast(&transmitter, &receiver, &discovery);
  
}

void loop() {
  fmtg packet;
  if (receiver.available())
  {
    receiver.read(&packet, sizeof(fmtg));
    if(!memcmp(packet.dst, addr, ADDR_S)){
      handleRecipient(&packet);
    } else if(!memcmp(packet.ir, addr, ADDR_S)){
      // handleReceiver(&packet);
    } else{
      printp(packet);
      // nonReceiver(&packet);
    }
  }

}

