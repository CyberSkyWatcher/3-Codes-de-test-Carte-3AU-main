#include <arduino.h>
#include <SPI.h>
#include "RF24.h"

// radio(CE, CS) ==> introduire les pin utilisées
// Sur carte 3AU 2.0      CE 15 //  CS 13
RF24 radio(15, 13); 

/* Ici sont listées les adresses des modules
dans cet exemple on travaille avec 2 modules*/
/*Tous les modules du réseau ont cette liste dans leur code*/
uint8_t address[][6] = {"1Node", "2Node"};

/*RadioNumber détermine quelle adresse donner à quel module*/
bool radioNumber = 0; 
/*role détermine si le module envoie ou  reçoit*/
bool role = false;  

/*Variables programme*/
char payload;
char msg[8];

void setup() {

  Serial.begin(9600);
  while (!Serial) {}      //attend l'ouverte du moniteur série

  // initialisation du module sur le bus SPI
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }

  Serial.println(F("Module initialisé"));

/******************************************************************/

  Serial.println(F("Introduire le numéro du module"));
  while (!Serial.available()) {
    // wait for user input
  }
  char input_number = Serial.parseInt();
  radioNumber = input_number;
  Serial.print(F("This is Node : "));
  Serial.println((int)radioNumber);

/******************************************************************/

  Serial.println(F("Introduire le role du module"));
  while (!Serial.available()) {
    // wait for user input
  }
  char input_role = Serial.parseInt();
  role = input_role;
  Serial.print(F("With role : "));
  Serial.println((int)radioNumber);

/******************************************************************/

// Set the PA Level low to prevent power supply related issues. RF24_PA_MAX is default.  
  radio.setPALevel(RF24_PA_LOW); 

// save on transmission time by setting the radio to only transmit the
  radio.setPayloadSize(sizeof(msg)); 

// set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

// set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

/******************************************************************/

  if (role) {
    radio.stopListening();  // put radio in TX mode
     Serial.println(F("Enter message"));
  } 
  else {
    radio.startListening(); // put radio in RX mode
  }
}



void loop() {
  if (role) {
    // This device is a TX node
    if (Serial.available()) {
        // wait for user input
      int i = 0;
      memset(msg,0,sizeof(msg));
      while(Serial.available()){
        char etr = Serial.read();
        msg[i] = etr;
        Serial.println(msg);
        i++;
      }
      unsigned long start_timer = micros();                    // start the timer
      bool report = radio.write(&msg, sizeof(msg));             // transmit & save the report
      unsigned long end_timer = micros();                      // end the timer
      i++;
      if (report) {
        Serial.print(F("Transmission successful! "));          // payload was delivered
        Serial.print(F("Time to transmit = "));
        Serial.print(end_timer - start_timer);                 // print the timer result
        Serial.print(F(" us. Sent: "));
        Serial.println(msg);                               // print payload sent
      } 
      
      else {
        Serial.println(F("Transmission failed or timed out")); // payload was not delivered
      }
      delay(100);  // slow transmissions down by 1 second
    
    }
  } 
  else {
  // This device is a RX node
    memset(msg,0,sizeof(msg));
    uint8_t pipe;
    if(radio.available(&pipe)){
      //uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      //char rcp;
      radio.read(&msg, sizeof(msg));            // fetch payload from FIFO
      //msg[i] = rcp;
      //i++;
      //Serial.println(rcp);                // print the payload's value
      //msg[i+1] = 0;
      Serial.println(msg);
    }
  }
}