/*!
@file measureVcc.h
@brief Functions used for measuring battery voltage.

@details The battery measurement software is taken from the Microchip application note AN2447
and uses the internal functions of the chip to access and read the chip's supply voltage.

Main problem encountered here was the ADC clock frequency
which wouldn't work with prescaler /8 (125KHz) but does
work with /16 (62.5KHz)
Also rather than sending a float back to the receiver (eg V = 3.1)
only high and low ADC values are sent (2 bytes).
Too much memory is used when calculating a float variable and it was thought better
to calculate the real value "off chip".

Note the conversion relies upon ADIF which is the interrupt complete flag.

@date Created: 02/01/2020 15:57:48
@author  Author: alanod
 */ 


#ifndef MEASUREVCC_H_
#define MEASUREVCC_H_
/*!
@param void 

Initialise the ADC

*/
void initVccADC(void);

/*!
@param void 

Read the battery voltage using the internal
battery measurement technique

*/
void readVcc(void);


#endif /* MEASUREVCC_H_ */