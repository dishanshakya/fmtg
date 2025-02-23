#include <RF24.h>
#include <printf.h>
#include <fmtg.h>
#include <utils.h>
#include <routing.h>
#include <ecc.h>

RF24 transmitter(5,6);
RF24 receiver(7,8);

state_t currentState = IDLE;

volatile uint8_t dialpad = 0;
volatile bool ring = false, calling = false, reject = false;
uint8_t channel = 20;

void dialer()
{
  noInterrupts();
  if(currentState == IDLE)
  {
    dialpad = (dialpad+1)%4;
    digitalWrite(0, dialpad&1);
    digitalWrite(1, dialpad&2);
  }
  else if(currentState == INCALL || currentState == RINGING){
    Serial.println("stop the call");
    reject = true;
  }
  interrupts();
}

void caller()
{
  noInterrupts();
  if(currentState == IDLE)
  {
    //start discovery
  }
  else if(currentState == RINGING)
  {
    //accept call
  }
  interrupts();
}


void setup(){

  //button pin configuration
 pinMode(2, INPUT_PULLUP);
 pinMode(3, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(3), dialer, FALLING);
 attachInterrupt(digitalPinToInterrupt(2), caller, FALLING);

  //led configuration
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
    
    assign_address(addr_n2);
    initRoutingTable();

    Serial.begin(115200);
    printf_begin();

    // Transmitter setup
    initRadio(&transmitter);
    transmitter.setChannel(20);
    transmitter.stopListening();
    //Receiver Setup
    initRadio(&receiver);
    receiver.setChannel(channel);
    receiver.openReadingPipe(0, addr);
    receiver.openReadingPipe(1, BROADCAST_ADDR);
    receiver.startListening();


    //  check nrf
    transmitter.printPrettyDetails();
    receiver.printPrettyDetails();

    // calling = true;

    // Discover node
    // broadcast_reconnect(discovery);
    // callAfterSeconds(wakeup);

    
}

void handleReconnect(fmtg *packet){
    stop100ms();
    Serial.println("routing table ma khojna paryo reconnect garna");
    int index = search(packet);
    if(index < 0){
      Serial.println("xaina raixa muji, feri rebroadcast handinxu");
        rebroadcast(packet);
    } else{
        Serial.println("xa xa, is khali xa ki xaina herna paryo");
        if(routing_table[index].is == 0){
            Serial.println("Inserting new IS into routing table");
            routing_table[index].is = packet->is;
            fmtg ack(packet->src, packet->dst, addr, packet->is, P_REC, channel, packet->ischannel);
            unicast(&transmitter, &ack);
        }
    }
}
void wakeup()
{
  receiver.openReadingPipe(1, BROADCAST_ADDR);
  Serial.println("yo print bhayo interrupt muji");
}

void rebroadcast(fmtg *packet){
  insertEntry(packet);
  packet->is = addr;
  Serial.println("rebroadcasting this packet:");
  printp(packet);
  broadcast(&transmitter, &receiver, packet);
  callAfterSeconds(wakeup);
}

void sendack(fmtg *packet){
    if(packet->src == addr){ 
        Serial.println("Own dick not allowed"); 
        return; 
    }
    fmtg ack(addr, packet->src, addr, packet->is, P_ACK, channel, packet->ischannel);
    Serial.println("sending this ack:");
    printp(&ack);
    *ownEntry = RoutingEntry(&ack);
    unicast(&transmitter, &ack);
    currentState = CONNECTING;
}

void broadcast_reconnect(fmtg *packet){
  stop100ms();
    fmtg reconnect(packet->src, packet->dst, addr, BROADCAST_ADDR, P_REC, channel, 0);
    broadcast(&transmitter, &receiver, &reconnect);
    printp(&reconnect);
    callAfterSeconds(wakeup);
}

void sample(){};

void handledata(fmtg *packet){

    if(packet->dst == addr){
      // Serial.println("destination mai raixu esko, playing on speaker");
      ownEntry->irpc = 0;
      ownEntry->ispc++;
      if(ownEntry->ispc > 200)
      {
        ownEntry->ir = 0;
      }
        // do someth8ihn with the data
        if(packet->payload[0] = 'R')
        {
          // Serial.println((char*)&packet->payload[1]);
          if(currentState == CONNECTING)
          {
            repeat100ms(startRing);
            currentState = RINGING;
          }
          
        }
        else if(packet->payload[0] == 'A')
        {
          currentState = INCALL;
        } else if(packet->payload[0] == 'N')
        {
          currentState = TERMINATED;
          Serial.println("Uthayena phone muji");
        }
    } else{
      Serial.println("data ta ho but destinatoin ma haina, relay garnu paryo, searching on routing table");
      displayTable();
       printp(packet);
        int index = search(packet);
        RoutingEntry *entry = &(routing_table[index]);
        if(index >= 0){
          Serial.println("xa table ma");
            if(routing_table[index].ir == 0){
               broadcast_reconnect(packet); 
            } else if(routing_table[index].is == 0){
               return;
            }
            if(packet->src == routing_table[index].src){
                packet->ir = routing_table[index].ir;
                routing_table[index].ispc = 0;
                routing_table[index].irpc++;
                if(routing_table[index].irpc && routing_table[index].irpc % 25 == 0)
                {
                  fmtg ping(entry->dst, entry->src, addr, entry->ir, P_DAT, channel, entry->irchannel);
                  ping.attachPayload('P', "pinging", 8);
                  printp(&ping);
                  unicast(&transmitter, &ping);
                }
                if(routing_table[index].irpc > 2000)
                {
                  routing_table[index].is= 0;
                }
            } else{
                packet->ir = routing_table[index].is;
                routing_table[index].irpc = 0;
                routing_table[index].ispc++;
                if(routing_table[index].ispc && routing_table[index].ispc % 25 == 0)
                {
                  fmtg ping(entry->src, entry->dst, addr, entry->is, P_DAT, channel, entry->ischannel);
                  ping.attachPayload('P', "pinging", 8);
                  printp(&ping);
                  unicast(&transmitter, &ping);
                }
                if(routing_table[index].ispc > 2000)
                {
                  routing_table[0].ir = 0;
                }
            }
            packet->is = addr;
            printp(packet);
            unicast(&transmitter, packet);
        }
        else Serial.println("table ma xaina, not my business");
    }
}

void broadcast_addr_case(fmtg *packet){
  Serial.println("received this packet on broadcast:");
  printp(packet);
    if(packet->dst == addr && packet->type == P_DISC){
        Serial.println("packet ko dest is me so sending ack");
        sendack(packet);
        displayTable();
        currentState == CONNECTING;
    } else{
        Serial.println("packet ko dest is not me, checking if it is rec pkt");
        if(packet->type == P_REC){
            Serial.println("reconnect ta raixa ni muji");
            handleReconnect(packet);
        } else{
            Serial.println("reconnect haina, yo dsicovery rebroadcast garnu paryo");
            rebroadcast(packet);
        }
    }
}

void relayack(fmtg *packet){
  Serial.println("relay garnu paryo yo ack, chekcing routing table");
    int index = search(packet);
    Serial.println(index);
    
    packet->ir = routing_table[index].is;
    routing_table[index].ir = packet->is;
    packet->is = addr;
    Serial.println("relaying this ack:");
    printp(packet);
    displayTable();
    unicast(&transmitter, packet);
}

void startRing()
{
  static int i = 0;
  if(i<3000)
  {
   ring = true;
    i++;
  } else {
    currentState = TERMINATED;
    stop100ms();
    ring = false;
  }

}
void own_addr_case(fmtg *packet){
    // Serial.println("unicast ack ho ki rec ho yo muji check garnu paryo");
    if(packet->type == P_ACK){
      Serial.println("ack raixa");
        if(currentState == CONNECTING && packet->dst == addr){
            Serial.println("Got an ack from the recepient. Start ringing");
            ownEntry->ir = packet->is;
            ownEntry->irchannel = packet->ischannel;
            displayTable();
            currentState = RINGING;
            repeat100ms(startRing);
        } else{
          Serial.println("destination ma haina raixa");
            relayack(packet);
        }
    } else if(packet->type ==  P_REC){
        Serial.println("rec raixa muji");
        int index = search(packet);
        if(routing_table[index].ir = 0){
            routing_table[index].ir = packet->is;
            Serial.println("feri paht banyo muji");
        } else{
            relayack(packet);
        }

    } else{
        // Serial.println("rec ni haina ack ni haina, data po raixa muji");
        handledata(packet);
    }
}

void loop(){
    fmtg packet, ringpacket;
    static int i = 0;
    byte buff[16] = "Ringing";

    if(currentState == IDLE && calling)
    {
       fmtg discovery(addr, addr_n2, addr, BROADCAST_ADDR, P_DISC, channel, 0);
       *ownEntry = RoutingEntry(&discovery);
        broadcast(&transmitter, &receiver, &discovery);
        currentState = CONNECTING;
        calling = false;
    }
    switch(currentState)
    {
      case RINGING:
        fmtg ringpacket(addr, ownEntry->dst, addr, ownEntry->ir, P_DAT, channel, ownEntry->irchannel);
        ringpacket.attachPayload('R', buff, sizeof(buff));
        if(ring)
        {
          unicast(&transmitter, &ringpacket);
          ownEntry->ispc = 0;
          ownEntry->irpc++;
          // displayTable();
          Serial.println(ownEntry->irpc);
          if(ownEntry->irpc > 200)
          {
            Serial.println("fuck");
            routing_table[0].ir = 0;
            broadcast_reconnect(&ringpacket);
          }
          // printp(ringpacket);
          ring = false;
        }
        if(reject)
        {
          stop100ms();
          currentState = TERMINATED;
          fmtg accept(addr, ownEntry->dst, addr, ownEntry->ir, P_DAT, channel, ownEntry->irchannel);
          accept.attachPayload('N', "i reject", 9);
          unicast(&transmitter, &accept);
          reject = false;
        }
        break;
      case INCALL:
      ;

    }

    if(receiver.available()){
        // Serial.println("Receiver Available, reading packet");
        receiver.read(&packet, sizeof(packet));
        if(packet.ir == BROADCAST_ADDR){
            Serial.println("pipe 1 (broadcast) ma aayo");
            broadcast_addr_case(&packet);
        } else if(packet.ir == addr){
            // Serial.println("pipe 0 (unicast) ma aayo");
            own_addr_case(&packet);
        }
    }
}


