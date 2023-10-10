#include <RF24.h>//https://github.com/nRF24/RF24
#include <nRF24L01.h>//https://github.com/nRF24/RF24/blob/master/nRF24L01.h
#include <SPI.h>

RF24 radio(12, 15); // instance nrf (cs,csn)

// Let these addresses be used for the pair
uint8_t address[1][2] = {0x01, 0x02};
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// 0 master 1 slave
bool radioNumber = 1; // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
//bool role = false;  // true = TX role, false = RX role


void setup_nrf(void){
        // initialize the transceiver on the SPI bus
    if (!radio.begin()) {
        Serial.println(F("radio hardware is not responding!!"));
        while (1) {} // hold in infinite loop
    }
    // print example's introductory prompt
        Serial.println(F("nrf24l01 started"));

    // save on transmission time by setting the radio to only transmit the
    // number of bytes we need 
        radio.setPayloadSize(sizeof(float)); // float datatype occupies 4 bytes

    // set the TX address of the RX node into the TX pipe
        radio.openWritingPipe(address[radioNumber]);     // always uses pipe 0

    // set the RX address of the TX node into a RX pipe
        radio.openReadingPipe(1, address[!radioNumber]); // using pipe 1

        radio.startListening(); // put radio in RX mode
    // For debugging info
        // printf_begin();             // needed only once for printing details
        // radio.printDetails();       // (smaller) function that prints raw register values
        // radio.printPrettyDetails(); // (larger) function that prints human readable dat
}

void radio_send_data(char dtype ,float data){
    radio.stopListening();  // put radio in TX mode
    bool report1 = radio.write(&dtype, sizeof(float));      // transmit & save the report
    bool report2 = radio.write(&data, sizeof(char));      // transmit & save the report

    if (report1&&report2) {
      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.println(dtype); 
      Serial.println(data);                              // print payload sent
    }
    else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
    }
    radio.startListening(); // put radio in RX mode
}

float radio_receive_data(void){
    char data;
    float payload;
    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      radio.read(&data, sizeof(data));
      if(data == 0x01){
        uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
        radio.read(&payload, bytes);            // fetch payload from FIFO
        Serial.print(F("Received "));
        Serial.print(bytes);                    // print the size of the payload
        Serial.print(F(" bytes on pipe "));
        Serial.print(pipe);                     // print the pipe number
        Serial.print(F(": "));
        Serial.println(payload);                // print the payload's value
        return payload;
      }
      if(payload == 0x01){
          
      }
      if(payload == 0x01){

      }
    }
}