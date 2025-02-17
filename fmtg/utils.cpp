#include "utils.h"
#include <TimerOne.h>

uint8_t *full_addr(byte addr[ADDR_S]){
        static byte address[FULL_ADDR_S+1];
        memset(address, '0', FULL_ADDR_S);
        memcpy(address, addr, ADDR_S);
        address[FULL_ADDR_S] = '\0';
        return address;
}

void assign_address(byte address[ADDR_S]){
	memcpy(addr, address, ADDR_S);
}

int addrcmp(const byte addr1[ADDR_S], const byte addr2[ADDR_S]){
    return !memcmp(addr1, addr2, ADDR_S);
}

void *addrcpy(byte addr1[ADDR_S], const byte addr2[ADDR_S]){
    return memcpy(addr1, addr2, ADDR_S);
}

void broadcast(RF24 *transmitter, RF24 *receiver, fmtg *packet){
    byte broadcast_addr[5];
    memcpy(broadcast_addr, full_addr(BROADCAST_ADDR), FULL_ADDR_S);
    transmitter->openWritingPipe(broadcast_addr);
    receiver->closeReadingPipe(1);
    transmitter->write(packet, sizeof(fmtg));
    byte broadcast_pipe[5];
    memcpy(broadcast_pipe, full_addr(BROADCAST_ADDR), FULL_ADDR_S);
}

void unicast(RF24 *transmitter, fmtg *packet){
    byte unicast_addr[5];
    memcpy(unicast_addr, full_addr(packet->ir), FULL_ADDR_S);
    transmitter->openWritingPipe(unicast_addr);
    transmitter->write(packet, sizeof(fmtg));
}

void printp(fmtg packet)
{
        char buff[ADDR_S+1];
        char payload[PAYLOAD_S+1];
        buff[ADDR_S] = '\0';
        payload[PAYLOAD_S] = '\0';
        Serial.print("Src: ");          Serial.println((char*)memcpy(buff, packet.src, ADDR_S));
        Serial.print("Dst: ");          Serial.println((char*)memcpy(buff, packet.dst, ADDR_S));
        Serial.print("Isnd: ");         Serial.println((char*)memcpy(buff, packet.is, ADDR_S));
        Serial.print("Irecv: ");        Serial.println((char*)memcpy(buff, packet.ir, ADDR_S));
        Serial.print("Type: ");         Serial.println((char)packet.type);
        Serial.print("Hop: ");          Serial.println((int)packet.hop);
        Serial.print("Payload: ");      Serial.println((char*)memcpy(payload, packet.payload, PAYLOAD_S));
}
void (*callbackFunction)();

volatile uint16_t interruptCounter = 0;
const uint16_t targetInterrupts = 3;

void callAfterSeconds(void (*func)()) {
  callbackFunction = func;
  interruptCounter = 0;
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 62500 - 1; // (16000000 / 1024) * 4 seconds = 62500

  TCCR1B |= (1 << WGM12);                  // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);     // Prescaler 1024

  TIMSK1 |= (1 << OCIE1A);                 // Enable Timer1 Compare Match A interrupt

  sei(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect) {
  interruptCounter++;
  if (interruptCounter >= targetInterrupts) {
    if (callbackFunction) {
      callbackFunction();
    }
    TIMSK1 &= ~(1 << OCIE1A); // Disable Timer1 interrupt
  }
}

void (*timer2Callback)();

int c = 0;
unsigned long t = 0;

void repeatThisShit(void (*func)(), unsigned long intervalMicros) {
  timer2Callback = func;

  cli(); 

  TCCR2A = 0; 
  TCCR2B = 0; 
  TCNT2 = 0;

  TCCR2A |= (1 << WGM21);

  TCCR2B |= (1 << CS22);

  unsigned long clockFrequency = 16000000; 
  unsigned long prescaler = 64;           
  unsigned long ticks = (intervalMicros * (clockFrequency / 1000000)) / prescaler;

  if (ticks > 255) {
    ticks = 255;
  }

  OCR2A = ticks - 1;

  TIMSK2 |= (1 << OCIE2A);

  sei();
}

ISR(TIMER2_COMPA_vect) {
  if (timer2Callback) {
    timer2Callback();
  }
}
