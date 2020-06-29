/*
* measure_temp.c
* Supply booster - Changed PWM ratio
* from 50% to slightly less to reduce
* resultant DC voltage. Now ~5V with
* 3 volt battery voltage
*
*
* Created: 15/01/2020 18:56:47
*  Author: alano
*/

#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#include "measureTemp.h"


extern uint8_t payload[];

/*!
@details

	Using 16 bit Timer/Counter1 
	TCCR1 (Timer Counter Control Reg.) setup: 
	A & B 8 bit registers are used as a single 16 bit control register (see data sheet).
	COM1A1 = Sets OC1A on match 
	WGM11, 12 & 13 = Sets Fast PWM ICR1
	CS10 = Sets Clock on prescaler (note clk = 1mHz)
	ICR1 (Input Capture Reg. 16 Bit) Sets frequency fck/10 = 100kHz
	OCR1A (Output Compare Reg. 8 Bit) Sets pulse width 10/5 = 1:1 Square wave

*/

void boostSupply(int8_t onOff){
	//PWM Set up
	if(onOff){
		DDRB |= _BV(PORTB1); //B1 set to output for timer channel A
		TCCR1A |= _BV(COM1A1)|_BV(WGM11);
		TCCR1B |= _BV(CS10) | _BV(WGM12) | _BV(WGM13);
		ICR1 = 10;
		OCR1A = 3;
	}
	else{
		TCCR1A = 0;
		TCCR1B = 0;
	}
}

/*!
@details

	ADMUX is set to it's default values:
	Internal 1.1V reference 
	ADC6 selected - this relates to pin PA0 (pin 19) 
	ADC (Control and Staus Reg.) 
	ADEN = 1, ADC is enable 
	ADPS2 = 1, ADC clock is \16 (slowest possible = 62.5kHz)
	
	4 samples of temperature are taken, placed into averageArray
	and then averaged prior to loading the payload array.
*/
void measureTemp(){
	uint8_t x = 0;
//	uint16_t averageTemp = 0;
	uint16_t average = 0;
//	uint8_t temp = 0;
	
	uint16_t xL = 0;
	uint16_t xH = 0;
	uint16_t averageArray[4];
	
	
	PRR &= ~_BV(PRTIM1); //power up timer 1
	PRR &= ~_BV(PRADC); //ensure ADC is powered
	//DIDR0 = _BV(ADC6D);
	ADCSRA = 0; //reset
	ADCSRA = _BV(ADEN) | _BV(ADPS2);
	ADMUX  = 0x06; //ADC6 Digital input disable
	boostSupply(1);
	_delay_ms(10); //allows boost voltage to reach steady state
//dummy conversions
	while(x < 2){  
		ADCSRA |= _BV(ADSC); //start conversion
		if(ADCSRA & (1 << ADIF)){
			xH = ADCL+CAL; //important - ADCL is read first (see data sheet)
			xH = ADCH;
			x++;
		}
	}
	x = 0;
	while(x < 4){  //do 4 conversions before sending data and average
		ADCSRA |= _BV(ADSC); //start conversion
		if(ADCSRA & (1 << ADIF)){
			xL = ADCL+CAL; //important - ADCL is read first (see data sheet)			
			xH = ADCH;
			xH = xH << 8;
			averageArray[x] = xH + xL;
			x++;
		}
	}
	for(uint8_t n = 0; n<4; n++){
			average += averageArray[n];
	}
	average = average/4;
	xH = average;
	xH = xH >> 8;
	payload[3] = (uint8_t)xH;
	payload[4] = (uint8_t)average;
	ADCSRA = 0; //switch off ADC
	boostSupply(0);
}