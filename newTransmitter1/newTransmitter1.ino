#include <TimerOne.h>
#include <RF24.h>
#include <printf.h>
#define size 1024


byte addr[] = "11111";
RF24 transmit(7,8);

volatile unsigned rear = 0, avail = 0, front = 0;
 byte micBuffer[32], buffer[32];

void setup(){
  // TCCR2B = TCCR2B & B11111000 | B00000001;
  ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));

    // Set prescaler to 32 (ADPS2 = 1, ADPS1 = 0, ADPS0 = 0)
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
  Serial.begin(115200);
  printf_begin();
  

  transmit.begin();
  transmit.stopListening();
  transmit.openWritingPipe(addr);
  transmit.setCRCLength(1);
  transmit.setDataRate(1);
  transmit.setAutoAck(0);
  transmit.setPALevel(RF24_PA_MAX);
  // transmit.enableDynamicAck();

  transmit.printDetails();

  Timer1.initialize(125);
  Timer1.attachInterrupt(sample);
  pinMode(11, OUTPUT);
}

void sample(){
  static unsigned i = 0;
  unsigned x = analogRead(A2)>>2;
  // static float x =0;
  // byte sinx = (sin(8*x)+1)*127;
  // analogWrite(9, sinx);
  // Serial.write(&sinx, 1);
  // x+= 0.1;
  // Serial.println(x);
  
  // // if (x < 155 && x > 135) x = 144;
 if(i<32 && !avail)
 {
  micBuffer[i] = x*2-206;
  i++;
 }
 else {
  i = 0;
  avail = 1;
    //  Serial.write((byte*)micBuffer, 32);
  // memcpy(buffer, micBuffer, 32);
 }
  
  // static float x = 0;
  // float sinx = sin(4* x);
  // int val = (sinx +1) * 127;
  
  // analogWrite(11, x);
  // Serial.println(x);
  // x += 0.1;
  

}
void loop(){
  byte black[32];
  byte bitch[] = "noise mf";
  // int time = micros();
  if(avail)
  {
    noInterrupts();
     transmit.startFastWrite(micBuffer, 32, 1);
    //  transmit.txStandBy();
     Serial.write((byte*)micBuffer, 32);
     avail = 0;
     interrupts();
//      Serial.write(black, 32);
   
  
  // time = micros()-time;
  // Serial.println(time);
  }
  
  
 
}
