#include <RF24.h>
#include <printf.h>
#include <fmtg.h>
#include <utils.h>
#include <routing.h>
#include <ecc.h>
#include <TimerOne.h>

RF24 transmitter(2,3);
RF24 receiver(5,4);

byte unicast_pipe[5];
byte broadcast_pipe[5];

void setup(){
    
    assign_address(addr_n1);
    initRoutingTable();

    Serial.begin(115200);
    printf_begin();

    // Transmitter setup
    transmitter.begin();
    transmitter.setDataRate(1);
    transmitter.setPALevel(3);
    transmitter.setCRCLength(2);
    transmitter.enableDynamicAck();
    // transmitter.setAutoAck(0);
    transmitter.stopListening();

    //Receiver Setup
    receiver.begin();
    receiver.setDataRate(1);
    receiver.setPALevel(3);
    receiver.setCRCLength(2);
    receiver.enableDynamicAck();
    // receiver.setAutoAck(0);
   
    memcpy(unicast_pipe, full_addr(addr), FULL_ADDR_S);
    memcpy(broadcast_pipe, full_addr(BROADCAST_ADDR), FULL_ADDR_S);
    receiver.openReadingPipe(0, unicast_pipe);
    receiver.openReadingPipe(1, broadcast_pipe);
    receiver.startListening();


    //  check nrf
    transmitter.printDetails();
    receiver.printDetails();

    // Discover node
    fmtg discovery = construct_discovery(addr_n3);
    broadcast(&transmitter, &receiver, &discovery);

    
}

void handleReconnect(fmtg *packet){
    Serial.println("routing table ma khojna paryo reconnect garna");
    int index = search(packet);
    if(index < 0){
      Serial.println("xaina raixa muji, feri rebroadcast handinxu");
        rebroadcast(*packet);
    } else{
        Serial.println("xa xa, is khali xa ki xaina herna paryo");
        if(addrcmp(routing_table[index].is, "\0\0")){
            Serial.println("Inserting new IS into routing table");
            addrcpy(routing_table[index].is, packet->is);
            fmtg ack = construct_ack(packet);
            ack.type = P_REC;
            unicast(&transmitter, &ack);
        }
    }
}
void wakeup()
{
  receiver.openReadingPipe(1, broadcast_pipe);
  Serial.println("yo print bhayo interrupt muji");
  Timer1.detachInterrupt();
  Timer1.stop();
}

void rebroadcast(fmtg packet){
  insertEntry(&packet);
  memcpy(packet.is, addr, ADDR_S);
  Serial.println("rebroadcasting this packet:");
  printp(packet);
  broadcast(&transmitter, &receiver, &packet);

}

void sendack(fmtg *packet){
    if(addrcmp(packet->src, addr)){ 
        Serial.println("Own dick not allowed"); 
        return; 
    }
    fmtg ack = construct_ack(packet);
    Serial.println("sending this ack:");
    printp(ack);
    insertEntry(packet);
    unicast(&transmitter, &ack);
}

void broadcast_reconnect(fmtg packet){
    fmtg reconnect = construct_reconnect(packet);
    broadcast(&transmitter, &receiver, &reconnect);
}

void handledata(fmtg *packet){

    if(addrcmp(packet->dst, addr)){
      Serial.println("destination mai raixu esko, playing on speaker");
        // do someth8ihn with the data
    } else{
      Serial.println("data ta ho but destinatoin ma haina, relay garnu paryo, searching on routing table");
        int index  = search(packet);
        if(index > 0){
          Serial.println("xa table ma");
            if(addrcmp(routing_table[index].ir, "\0\0")){
               broadcast_reconnect(*packet); 
            } else if(addrcmp(routing_table[index].ir, "\0\0")){
               return;
            }
            if(addrcmp(packet->src, routing_table[index].src)){
                memcpy(packet->ir, routing_table[index].ir, ADDR_S);
            } else{
                memcpy(packet->ir, routing_table[index].is, ADDR_S);
            }
            memcpy(packet->is, addr, ADDR_S);
            unicast(&receiver, packet);
        }
        Serial.println("table ma xaina, not my business");
    }
}

void broadcast_addr_case(fmtg *packet){
  Serial.println("received this packet on broadcast:");
  printp(*packet);
    if(addrcmp(packet->dst, addr) && packet->type == P_DISC){
        Serial.println("packet ko dest is me so sending ack");
        sendack(packet);
    } else{
        Serial.println("packet ko dest is not me, checking if it is rec pkt");
        if(packet->type == P_REC){
            Serial.println("reconnect ta raixa ni muji");
            handleReconnect(packet);
        } else{
            Serial.println("reconnect haina, yo dsicovery rebroadcast garnu paryo");
            rebroadcast(*packet);
        }
    }
}

void relayack(fmtg *packet){
  Serial.println("relay garnu paryo yo ack, chekcing routing table");
    int index = search(packet);
    addrcpy(packet->ir, routing_table[index].is);
    addrcpy(routing_table[index].ir, packet->is);
    addrcpy(packet->is, addr);
    Serial.println("relaying this ack:");
    printp(*packet);
    unicast(&transmitter, packet);
}

void own_addr_case(fmtg *packet){
    Serial.println("unicast ack ho ki rec ho yo muji check garnu paryo");
    if(packet->type == P_ACK){
      Serial.println("ack raixa");
        if(addrcmp(packet->dst, addr)){
            Serial.println("Got an ack from the recepient");
            printp(*packet);
        } else{
          Serial.println("destination ma haina raixa");
            relayack(packet);
        }
    } else if(packet->type ==  P_REC){
        Serial.println("rec raixa muji");
        int index = search(packet);
        if(addrcmp(routing_table[index].ir, "\0\0")){
            addrcpy(routing_table[index].ir, packet->is);
            Serial.println("feri paht banyo muji");
        } else{
            relayack(packet);
        }

    } else{
        Serial.println("rec ni haina ack ni haina, data po raixa muji");
        handledata(packet);
    }
}

void loop(){
    fmtg packet;
    uint8_t pipe;

    if(receiver.available(&pipe)){
        Serial.println("Receiver Available, reading packet");
        receiver.read(&packet, sizeof(packet));
        if(pipe){
            Serial.println("pipe 1 (broadcast) ma aayo");
            broadcast_addr_case(&packet);
        } else{
            Serial.println("pipe 0 (unicast) ma aayo");
            own_addr_case(&packet);
        }
    }
}


