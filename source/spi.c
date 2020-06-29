/*
 * spi.c
 *
 * Created: 22/11/2019 13:01:06
 *  Author: alano
 */ 

#include <avr/io.h>
#include "spi.h"

void initSPI(void){
	PRR &= ~_BV(PRSPI); //ensure SPI h/w is powered
	DDRB |= _BV(SPI_SCK)|_BV(SPI_MOSI)|_BV(SPI_SS)|_BV(CE)|_BV(CSN); //set spi output pins
	SPCR |= _BV(SPE)|_BV(MSTR); //enable SPI and set to master
	SPCR &= ~_BV(CPOL)|_BV(CPHA); //set to mode 0
	
	CE_L;
	CSN_H;
}

uint8_t writeByteSPI(uint8_t cData){
	SPDR = cData;
	while(!(SPSR&_BV(SPIF)));
	return SPDR;
}