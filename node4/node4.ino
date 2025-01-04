#include <RF24.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <fmtg.h>

RF24 transmitter(2,3);
RF24 receiver(4,5);

int c;

void handleReceiver(fmtg* packet){}

void handleRecepient(){}

void nonReceiver(){}

void setup() {
  
  transmitter.begin();
  receiver.begin();
  Serial.begin(115200);
  printf_begin();

  transmitter.setDataRate(1);
  receiver.setDataRate(1);
  transmitter.setCRCLength(2);
  transmitter.setPALevel(3);

  receiver.openReadingPipe(0, addr);
  receiver.openReadingPipe(1, BROADCAST_ADDR);
  receiver.startListening();
  transmitter.stopListening();

  // transmitter.printDetails();
  // receiver.printDetails();

  // byte dest[5] = "1100";
  // byte ir[5] = "ff000";
  // fmtg discovery = construct_discovery(dest);
  // transmitter.openWritingPipe(dest);

  // for(int i = 1; i <= 1000; i++){
  //   transmitter.write(&discovery, sizeof(fmtg));
  //   delay(100);
  // }

  c = 0;
  
  
}

void loop() {
  fmtg packet, trash;
  if (receiver.available())
  {
    receiver.read(&packet, sizeof(fmtg));
    printp(packet);
    Serial.println("----------------------");
    Serial.println(++c);
  }

}
