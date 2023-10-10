/************CAN - Receiver avec callback ************/
/* RTR ON --> sur demande renvoie le contenu du sending buffer */
#include <arduino.h>
#include <CAN.h>

bool flag;  // variable mise à lors de la réception
void onReceive(int packetSize);

char sending_buffer[8] = {'R','I','S','O','K'};
void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("CAN Receiver Callback");

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // register the receive callback
  CAN.onReceive(onReceive);
}

void loop() {
  // do nothing
  if(flag){
  CAN.beginPacket(0x12);
  /*CAN.write('l');
  CAN.write('e');
  CAN.write(0x20);
  CAN.write('B');
  CAN.write('G');*/
  for(int i = 0; i<=7 ;i++){
    CAN.write(sending_buffer[i]);
  }
  CAN.endPacket();
  flag = 0;
  }
}

void onReceive(int packetSize) {
/* Fonction de callback --> appelée lorsque qq chose arrive via CAN*/
  Serial.print("Received ");

  if (CAN.packetExtended()) {
    Serial.print("extended ");
  }

  if (CAN.packetRtr()) {
    // Remote transmission request
    Serial.print("RTR ");
    flag = 1;
  }

  Serial.print("packet with id 0x");
  Serial.print(CAN.packetId(), HEX);

  if (CAN.packetRtr()) {
    Serial.print(" and requested length ");
    Serial.println(CAN.packetDlc());
  } else {
    Serial.print(" and length ");
    Serial.println(packetSize);

    // Si des données sont reçues et que ça n'est pas un RTR
    // Alors on affiche le message
    while (CAN.available()) {
      Serial.print((char)CAN.read());
    }
    Serial.println();
  }
}



