#include <RF24.h>
#include <SPI.h>
#include <printf.h>
#include <TimerOne.h>
#define size 1024
// #include <fmtg.h>

byte broadcast[5] =  "ff000";
byte addr[5] = "11111";

RF24 transmitter(7,8);
RF24 receiver(5,6);
volatile byte audioBuffer[32];

volatile unsigned occupied = 0;






void play()
{
  static unsigned chunkIndex = 0, i =0;
  if(i<32)
  {
    byte n = audioBuffer[i];
    // Serial.write(&n, 1);
    analogWrite(3, n);
    i++;
  }
  else {
    i = 0;
    occupied = 0;
    // memset(audioBuffer, 'g', 32);
  }
 
  

//  if(Serial.available())
//  {
//    byte n= Serial.read();
////    Serial.println(45);
  //  analogWrite(3, n);
//  }
}

void setup() {
  Serial.begin(115200);
  printf_begin();
  pinMode (3,OUTPUT);

  transmitter.begin();
  transmitter.setDataRate(1);
  transmitter.setCRCLength(2);
  transmitter.stopListening();

  TCCR2A = 0;                // Clear control register A
  TCCR2B = 0;                // Clear control register B

  // Set Fast PWM mode (Mode 3: WGM21 and WGM20 set)
  TCCR2A |= (1 << WGM20) | (1 << WGM21);

  // Non-inverting mode for OC2B (Pin 3)
  TCCR2A |= (1 << COM2B1);

  // No prescaler (CS20 set)
  TCCR2B |= (1 << CS20);

  // Set the duty cycle (e.g., 50%)
  // OCR2B = 127;
  
  receiver.begin();
  receiver.setDataRate(1);
  receiver.setCRCLength(2);
  receiver.openReadingPipe(0, addr);
  // receiver.openReadingPipe(1, broadcast);
  receiver.startListening();
  receiver.setAutoAck(0);

  receiver.printDetails();
//  analogWrite (3,255);
  Timer1.initialize(125);
  Timer1.attachInterrupt(play);

  // transmitter.openWritingPipe(broadcast);

}
// int time = micros();
// time = micros() - time;
// Serial.println(time);
void loop() {
  byte black[32];
  static unsigned chunkIndex = 0;
    if (receiver.available() && !occupied)
    {
      noInterrupts();
      receiver.read(black, 32);
      // Serial.write(black, 32);
      memcpy(audioBuffer, black, 32);
      occupied=1;
      interrupts();
//      for(int i=0; i<32; i++)
//      {
//        if((rear+1)%size != front)
//        {
//          chunk[(rear+1)%size] = black[i];
//          rear = (rear+1)%size;
//        }
////        else delayMicroseconds(125);
//      }
      
	  }   
      
    

}
