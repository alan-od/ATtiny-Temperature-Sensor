/*
 * ATtiny48_FullSensor_v3.c
 *
 * Created: 27/01/2020 12:32:43
 * Author : alano
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/common.h>
#include <avr/wdt.h>
#include <stdio.h>
#include "main.h"
#include "nRF24L01.h"
#include "spi.h"
#include "nRF24.h"
#include "measureTemp.h"
#include "measureVcc.h"

#define LED_OFF (PORTD &= ~_BV(PORTD5))
#define LED_ON  (PORTD |= _BV(PORTD5))

//required for power management
#define DELAY_COUNT 3 //gives ~ 40 sec. delay

uint8_t timerCounter; ///sets no of times watchdog will trigger before temperature measured
uint8_t vccCounter;  ///determine when Vcc will be measured
uint8_t payload[6] = {0x01,0x10,0x02,0xF0,0x30,0xff};
uint8_t count = 0; ///tx sequence number

/*! @details Payload array structure
	payload[0] = Unit number
	payload[1] = Battery Voltage ADCH
	payload[2] = Battery Voltage ADCL
	payload[3] = Temperature ADCH
	payload[4] = Temperature ADCL
	payload[5] = Count  0 to 255;
	payload[5] =
	payload[6] =
	payload[7] =
*/


//Initialise interrupt
void Int0Initialise(){
	DDRD &= ~(_BV(PORTD2));  //sets D2 to input (is input by default)
	EICRA |= (_BV(ISC01));   //triggers on falling edge
	EIMSK |= (_BV(INT0));    //enables INT0
}
/*!

*/
/*!
Power management - watchdog fires interrupt at timeout (8s).
*/
ISR(WDT_vect){
}
/*!
NRF24 fires interrupt on successful ack received from remote rx.
*/
ISR(INT0_vect){
	cli();
	CE_L;
	count++; //let count roll over (uint)
	payload[5] = count; 
	reset();
	sei();
}

//set up watchdog counter for power saving functionality
//Watchdog times out and fires interrupt WDT_vect.
void wdtEnable(){
	wdt_reset();
	PRR |= _BV(PRADC) | _BV(PRTIM0) | _BV(PRTIM1) | _BV(PRSPI) ; //switches off unused functions

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode();
	sleep_disable();
}

void setupWDT(){
	cli();
	wdt_reset();
	WDTCSR |= _BV(WDCE)| _BV(WDE);
	WDTCSR =  _BV(WDIE) | _BV(WDP3) | _BV(WDP0); //see data sheet - ensure WDIE is enabled not WDE
	//shut down brown out						 //use WDP2 & WDP1 to test, 3 & 0 for max delay 
	MCUCR |= _BV(BODS) | _BV(BODSE);
	sei();
}



/*******************************
Test Led
*******************************/

void fireLed(){
	DDRD |= _BV(PORTD5); //D5 set to output for LED
	LED_ON;
	_delay_ms(10);  //10mS, can be shorter but a bit dim
	LED_OFF;
	DDRD &= ~_BV(PORTD5); //set to input
}

/*******************************
End Test Led
*******************************/


int main(void)
{
uint8_t powerDownCounter = 100;
uint8_t measureVccDelay = 0;

	//Set up unused pins
	DIDR0 = 0xFF; //Digital input disable
	//Set unused pins to inputs and enable pull ups
	PORTC &= ~(_BV(PORTC6) | _BV(PORTC7));
	DDRC |= _BV(PORTC6) | _BV(PORTC7);
	PORTD &= ~(_BV(PORTD0) | _BV(PORTD1) | _BV(PORTD3) | _BV(PORTD4) | _BV(PORTD6) | _BV(PORTD7));
	DDRD |= _BV(PORTD0) | _BV(PORTD1) | _BV(PORTD3) | _BV(PORTD4) | _BV(PORTD6) | _BV(PORTD7);
	setupWDT();
	Int0Initialise();
	readVcc();
	payload[0] = UNIT_NO;
	
	while(1){
		if(powerDownCounter > PWR_COUNTER){
			measureTemp();
			txPayload(payload);
			fireLed();
			if(measureVccDelay > 20){
				readVcc();			
				measureVccDelay = 0;
			}
			powerDownCounter = 0;
			measureVccDelay++;
		}
	powerDownCounter++;	
	wdtEnable();
	}
}
	
	





