/*!
 @file main.h
 @brief Main plus interrupt handler. 
 
 @details
 No much to say
  *
 * @date 17/01/2020 16:09:47
 * @author: alano
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/************************************************************************/
/* 
	General Macros
                                                                     */
/************************************************************************/

/*!
Temperature Calibration
ADC is 10 bit representing 1024 states
ADC reference voltage = 1.1V, hence each bit = 1.1/1024 volts (~1.07mV)
Sensors output is 20mV per degree C. so 0.5C represents a count of around 9.
Add or subtract 9 to ADCL to calibrate sensor to 0.5C (measureTemp.c)
*/
#define CAL (-9) //9


/*! 
	PWR_COUNTER
	Sets time between temperature readings. With WDT at 8 second delay
	(WDP3 and WDP0) a value of 6 gives about 1 minute 10s between readings (noticed 128kHz 
	internal oscillator on development processor ran slow).
*/
#define PWR_COUNTER 6 
/*!
	UNIT_NO
	This number sets the number of the temperature sensor device, there may be
	more than one in a system - a uint8_t
*/
#define UNIT_NO 1
/*!
	Set up receive address - 5 bytes
*/
#define RECEIVE_ADDRESS		0x12,0x12,0x12,0x12,0x12
/*!
	Set up transmit address - 5 bytes
*/
#define TRANSMIT_ADDRESS	0x12,0x12,0x12,0x12,0x12

void Int0Initialise(void);
void wdtEnable(void);
void setupWDT(void);



#endif /* MAIN_H_ */