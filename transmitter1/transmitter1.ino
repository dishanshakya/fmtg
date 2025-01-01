#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <fmtg.h>

byte broadcast[5] =  "ff000";
byte addr[5] = "22000";

RF24 transmitter(2,3);
RF24 receiver(5,4);


void handleReceiver(fmtg* packet){}

void handleRecepient(){}

void nonReceiver(){}

// void listen(){
//   fmtg *packet = malloc(sizeof(fmtg));
//   if(receivr.available()){
//     radio.read(packet, sizeof(fmtg));

//     switch(packet->type){

//       case 'D':
//       if(packet->dest == addr){
//         handleRecepient(packet);
//       } else if(packet->ir == addr){
//         handleReceiver(packet);
//       } else{
//         nonReceiver(packet);
//       }

//       break;

//       case 'A':
//       break;

//       case 'd':
//       break;

//     }
//   }

// }

void setup() {
  transmitter.begin();
  receiver.begin();
  Serial.begin(115200);
  printf_begin();
  transmitter.setDataRate(1);
  receiver.setDataRate(1);
  transmitter.setCRCLength(2);
  receiver.openReadingPipe(0, addr);
  receiver.openReadingPipe(1, broadcast);
  receiver.startListening();
  transmitter.stopListening();

  transmitter.printDetails();
  receiver.printDetails();
  byte recv[5] = "11000";
  transmitter.openWritingPipe(broadcast);

  for(int j=0; j<2; j++){
  fmtg packet = construct_discovery(addr, recv,0);
  transmitter.write(&packet, sizeof(packet));
  receiver.read(&packet, sizeof(fmtg));}
  
  // fmtg niggapacket = construct_discovery(recv);
  // transmitter.openWritingPipe(recv);
  // transmitter.write(&niggapacket, sizeof(niggapacket));
}

void loop() {
  fmtg packet, trash;
  if (receiver.available())
  {
    Serial.println("av");
    receiver.read(&packet, sizeof(fmtg));
    printp(packet);
  }
  //   if(strcmp(packet.src, "22") == 0){
      
  //   }
  //   else if(strcmp(packet.ir, "ff") == 0){
  //     Serial.println("Rebroadcasting...");
  //     fmtg rebroadcast = construct_discovery(broadcast,++packet.hop);
  //     transmitter.write(&rebroadcast, sizeof(rebroadcast));
  //     receiver.read(&trash, sizeof(packet));
  //     transmitter.write(&rebroadcast, sizeof(rebroadcast));
  //     receiver.read(&trash, sizeof(packet));
  //   }
    
  // }
}
