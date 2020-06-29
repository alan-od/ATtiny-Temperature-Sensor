/*
 * nRF24.h
 *
 * Created: 31/12/2019 12:48:27
 *  Author: alano
 */ 


#ifndef NRF24_H_
#define NRF24_H_

#define DELAY 5

uint8_t *readWrite(uint8_t, uint8_t, uint8_t*, uint8_t);
void txPayload(uint8_t *);
/*!
Initialises nRF24 module. Sets Channel number (100) and address 0x12 by 5
Also appends status byte into 

*/
void nrfInit(void);
uint8_t getReg(uint8_t reg);
void reset(void);



#endif /* NRF24_H_ */