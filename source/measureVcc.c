/*!
@details
* measureVcc.c
*
Measure battery voltage
Taken from application sheet AN2447

	Main problem encountered here was the ADC clock frequency
	Wouldn't work with prescaler /8 (125KHz) but does
	work with /16 (62KHz)
	
	Also rather than sending a float back to receiver (eg V = 3.1)
	only high and low ADC values are sent (2 bytes).
	Too much memory was used when calculating a float variable.
	
	Note the conversion relies upon ADIF which is the interrupt complete flag
	but I didn't set up  ADIE and SREG (see data sheet) but it seems to work.

* Created: 02/01/2020 15:57:17
*  Author: alano
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

extern uint8_t payload[];
/*!
@details

	PRR register bit PRADC bit is set to 0 to ensure the ADC is powered up. 
	ADCSRA and ADMUX registers are reset to 0 to ensure a clean start up.
	ADCSRA bits ADEN (ADC enable) and (ADPS2) are set to 1 and the ADMUX register
	is set to values as detailed in application note. 
	
*/


void initVccADC(){
	PRR &= ~_BV(PRADC); //Ensure ADC is powered up
	ADCSRA = 0; //set registers to default values
	ADMUX = 0;
	ADCSRA = _BV(ADEN)|_BV(ADPS2);      //Tried prescaler /8 = 125kHz didn't work
										//with 1MHz clock (/16) and it works!
	ADMUX = (0x01 << REFS0) | (0x0E << MUX0); //0x4E ref = vcc, input v = Vref
}

/*!
@details

	Rather than relying upon a single ADC conversion I decided to run 3 conversions
	to ensure there weren't any startup problems.
	Note the ADC register read sequence,

*/

void readVcc(){
	initVccADC();
	_delay_ms(2);
	uint8_t x = 0;
	
	while(x < 3 ){  //do a few conversions before loading data
		ADCSRA |= _BV(ADSC); //start conversion
		if(ADCSRA & (1 << ADIF)){
			payload[2] = ADCL; //ensure correct read sequence is used
			payload[1] = ADCH; 
			x++;
		}
	}
	ADCSRA = 0; 
}
	
//	Do this in the receiver
//	vccValue = (0x400*1.1)/(adcRes_L+adcRes_H * 0x100);
	//return byte array representation of float
//	for(uint8_t i = 0; i < 4; i++){
//	payload[i] = ((uint8_t*)&vccValue)[i];
//	}
	
 