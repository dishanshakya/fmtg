#include "utils.h"
#include <TimerOne.h>

void initRadio(RF24 *radio)
{
	radio->begin();
	radio->setDataRate(1);
	radio->setCRCLength(2);
	radio->setPALevel(3);
	radio->enableDynamicAck();
	radio->setAutoAck(0);
}

void assign_address(uint16_t address){
	addr = address;
}

void broadcast(RF24 *transmitter, RF24 *receiver, fmtg *packet){
	noInterrupts();
    transmitter->openWritingPipe(BROADCAST_ADDR);
    receiver->closeReadingPipe(1);
    for(int i=1; i<4; i++)
    {
	    transmitter->setChannel(i * 10);
	    transmitter->writeFast(packet, sizeof(fmtg));
    }
    interrupts();
}

void unicast(RF24 *transmitter, fmtg *packet){
	noInterrupts();
	transmitter->setChannel(packet->irchannel);
    transmitter->openWritingPipe(packet->ir);
    transmitter->writeFast(packet, sizeof(fmtg));
    interrupts();
}

void printp(fmtg *packet)
{
        char payload[PAYLOAD_S+1];
        payload[PAYLOAD_S] = '\0';
        Serial.print("Src: ");          Serial.println(packet->src, HEX);
        Serial.print("Dst: ");          Serial.println(packet->dst, HEX);
        Serial.print("Isnd: ");         Serial.println(packet->is, HEX);
        Serial.print("Irecv: ");        Serial.println(packet->ir, HEX);
        Serial.print("Type: ");         Serial.println((char)packet->type);
        Serial.print("Hop: ");          Serial.println((int)packet->hop);
	Serial.print("Ischannel: ");	Serial.println((int)packet->ischannel);
	Serial.print("Irchannel: ");	Serial.println((int)packet->irchannel);
        Serial.print("Payload: ");      Serial.println((char*)memcpy(payload, packet->payload, PAYLOAD_S));
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

  OCR1A = 62500 - 1; // (16000000 / 256) * 4 seconds = 62500

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

void (*timer2Callback)(), (*mscallback)();
volatile byte overflow = 0, count = 0;

void repeat(void (*func)(), unsigned long freq) {
  timer2Callback = func;

  cli();

  TCCR2A = (1 << WGM21);

  TCCR2B =  (1 << CS21) ;

  unsigned long clockFrequency = 16000000;
  unsigned long prescaler = 8;

  OCR2A = clockFrequency/(prescaler*freq)-1;

  TIMSK2 = (1 << OCIE2A);

  sei();
}

void repeat100ms(void (*func)()) {
	mscallback = func;
	cli();
	count = 6;
	TCCR2A = 0;
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);
	TIMSK2 = (1 << TOIE2);
	sei();
}

void stop100ms(){
	TIMSK2 &= ~(1 << TOIE2);
}

void stopRepeat(){
	TIMSK2 &= ~(1 << OCIE2A);
}
	

ISR(TIMER2_OVF_vect) {
	overflow++;
	if (mscallback && overflow >= count){
		mscallback();
		overflow = 0;
	}
}


ISR(TIMER2_COMPA_vect) {
  if (timer2Callback) {
    timer2Callback();
  }
}
