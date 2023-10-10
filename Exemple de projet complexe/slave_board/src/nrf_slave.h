#ifndef NRF_SLAVE_H
#define NRF_SLAVE_H

void setup_nrf(void);
void radio_send_data(char dtype ,float data);
float radio_receive_data(void);
#endif