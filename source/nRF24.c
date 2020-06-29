/*
 * nRF24.c
 *
	These are the functions required to control the
	nRF24 transceiver.
 
 * Created: 31/12/2019 12:46:19
 *  Author: alano
 */ 
#include <avr/interrupt.h>
#include <util/delay.h>
#include "spi.h"
#include "nRF24L01.h"
#include "nRF24.h"
#include "main.h"


uint8_t rxAddArray[5] = {RECEIVE_ADDRESS};
uint8_t txAddArray[5] = {TRANSMIT_ADDRESS};
	
uint8_t *readWrite(uint8_t rw, uint8_t reg, uint8_t *val, uint8_t length){
	static uint8_t ret[10]; //10 byte max data packet big enough for our needs
	if(rw == W){
		reg = W_REGISTER+reg;
	}
	_delay_us(DELAY);
	CSN_L;
	_delay_us(DELAY);
	writeByteSPI(reg);
	_delay_us(DELAY);
	
	for(uint8_t i = 0; i<length; i++){
		if(rw == R && reg != W_TX_PAYLOAD){
			ret[i]= writeByteSPI(NOP);
			_delay_us(DELAY);
		}
		else{
			writeByteSPI(val[i]);
			_delay_us(DELAY);
		}
	}
	CSN_H;
	return ret;
}

void nrfInit(void){
	uint8_t val[1];  //bytes to send
	//Enable auto-ack
	val[0]=0x01;
	readWrite(W,EN_AA,val,1);
	
	//Set up pipe number (1-5)
	val[0]=0x01;
	readWrite(W,EN_RXADDR,val,1);

	//Set address width to 5
	val[0]=0x03;
	readWrite(W,SETUP_AW,val,1);
	
	//Set RF channel
	val[0]=100; //channel 100
	readWrite(W,RF_CH,val,1);
	
	//Set Power and Data rate
	val[0]=0x06; // 0000,0011 bits 0 & 1 = power; bit 3 = high or low 2M, 1M
	//val[0]=0x26; //Sets data rate to 250Kb and power to 0dBm
	readWrite(W,RF_SETUP,val,1);
	
	//Set RX address - 5 byte address
	readWrite(W, RX_ADDR_P0, rxAddArray,5);
	
	//Set up TX address - 5 bytes
	readWrite(W, TX_ADDR, txAddArray,5);
	
	//Payload width 1 - 32 bytes
	val[0] = 6; //sets data package to 5 bytes
	readWrite(W, RX_PW_P0, val,1);
	
	//Set up retries and frequency between retires
	val[0] = 0x25; //2 sets to 750uS between retries 500uS min for 1Mbs setting
	//lower 4 bits = no. of retries
	readWrite(W, SETUP_RETR, val,1);
	
	//CONFIG reg setup. Set to RX or TX
	val[0] = 0x1E;
	//Bit 0 - 0 for TX, 1 for RX.
	//Bit 1 - 1 for Power On, 0 for Power Off
	//Bit 2 - 1 16 bit CRC, 0 8 bit CRC
	//Bit 3 - 1 Enable CRC
	//-----
	//Bit 4 - 1 MAX_RT Mask Interrupt (transmission failed)
	//Bit 5 - 0 (default) TX_DS
	//Bit 6 - 0 (default) RX_DS
	readWrite(W, CONFIG, val,1);
}

void txPayload(uint8_t *writeBuf){
	initSPI();
	nrfInit();

	readWrite(R,FLUSH_TX,writeBuf, 0); //Flush old data
	readWrite(R,W_TX_PAYLOAD,writeBuf,6);
	reset(); //if nrf24 goes out of range it hangs so reset after tx - result of power saving measures 
	sei();
	_delay_ms(DELAY);
	CE_H;
	_delay_us(DELAY);
	CE_L;
	_delay_ms(DELAY);
}

uint8_t getReg(uint8_t reg){
	_delay_us(DELAY);
	CSN_L;
	_delay_us(DELAY);
	writeByteSPI(R_REGISTER+reg);
	_delay_us(DELAY); //doesn't like a delay here - between multiple bytes
	reg = writeByteSPI(NOP);
	_delay_us(DELAY);
	CSN_H;
	return reg;
}

void reset(void){
	_delay_us(DELAY);
	CSN_L;
	_delay_us(DELAY);
	writeByteSPI(W_REGISTER+STATUS);
	_delay_us(DELAY);
	writeByteSPI(0x70);
	_delay_us(DELAY);
	CSN_H;
}
