#include <RF24.h>
#include <SPI.h>
#include <printf.h>
#include <TimerOne.h>
#define size 1024
// #include <fmtg.h>

byte broadcast[5] =  "ff000";
byte addr[5] = "11111";

RF24 transmitter(7,8);
RF24 receiver(2,3);
volatile byte audioBuffer[32];

volatile unsigned occupied = 0;
void play()
{
  static unsigned chunkIndex = 0, i =0;
  if(i<32)
  {
    byte n = audioBuffer[i];
    // Serial.write(&n, 1);
    analogWrite(9, n);
    analogWrite(10, 255-n);
    i++;
  }
  else {
    i = 0;
    occupied = 0;
  }
}

void setup() {
  Serial.begin(115200);
  printf_begin();
  pinMode (3,OUTPUT);

  transmitter.begin();
  transmitter.setDataRate(1);
  transmitter.setCRCLength(2);
  transmitter.stopListening();

   pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);  // Set D9 as output

  // Configure Timer 1
  TCCR1A = _BV(COM1A1) | _BV(WGM11);   // Fast PWM, Clear on Compare Match (Non-inverting)
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // Fast PWM, No Prescaler (CS10 = 1)
  
  ICR1 = 31;
  
  receiver.begin();
  receiver.setDataRate(0);
  receiver.setCRCLength(1);
  receiver.openReadingPipe(0, addr);
  // receiver.openReadingPipe(1, broadcast);
  receiver.startListening();
  receiver.enableDynamicAck();
  receiver.setAutoAck(0);
  // receiver.setPALevel(RF24_PA_LOW);

  receiver.printDetails();
//  analogWrite (3,255);
  Timer1.initialize(125);
  Timer1.attachInterrupt(play);

}
void loop() {
  byte black[32];
  static unsigned chunkIndex = 0;
    if (receiver.available() && !occupied)
    {
      noInterrupts();
      receiver.read(black, 32);
      Serial.write(black, 32);
      memcpy(audioBuffer, black, 32);
      occupied=1;
      interrupts();
      
	  }   
      
    

}
