/*!
 @file spi.h
 @brief SPI
 
 @details These functions set up the SPI pins CE, CSN, MISO, MOSI and SS, and
 reads and writes to the SPI register.
 
 
 One of the "gotchas" here is ensuring SS is set to an output.  If this 
 pin is not set the SPI unit wont work, even though the the SS pin may not be used.

 @author alano
 @date Created: 22/11/2019 13:00:26
 
 */ 



#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

#define SPI_DDR	 DDRB
#define SPI_PORT PORTB
#define SPI_MISO PORTB4
#define SPI_MOSI PORTB3
#define SPI_SCK	 PORTB5
#define SPI_SS	 PORTB2
#define CE		 PORTB7
#define CSN		 PORTB6

#define CE_H	SPI_PORT |= _BV(CE)
#define CE_L	SPI_PORT &= ~_BV(CE)

#define CSN_H	SPI_PORT |=_BV(CSN)
#define CSN_L	SPI_PORT &= ~_BV(CSN)

/*!
@param void


Initialise the SPI unit - sets up pins for output, SPI clock frequency
and enables the SPI.

*/
void	initSPI(void);
/*!
@param uint8_t data
		uint8_t return

Write data to the SPI register. 
Returns data clocked out from the SPI data register. Note to read the SPI register a NULL is
sent as a data byte e.g. writeByteSPI(NULL).

*/
uint8_t writeByteSPI(uint8_t);



#endif /* SPI_H_ */