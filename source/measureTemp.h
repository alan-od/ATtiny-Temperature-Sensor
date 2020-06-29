/*!
 @file measureTemp.h
 @brief Functions used for measuring temperature.
 
 @details Because the temperature sensor (TMP37) lowest operating voltage
 is 2.7V and the supply voltage can drop below this a supply booster
 is used to bring the 'sensor's supply voltage to around 4/5V before
 an ADC measurement is made.
 
 To raise the supply voltage a 100kHz square wave is generated using
 Timer 1.

 @author AOD
 @date Created: 15/01/2020

 */ 



#ifndef MEASURE_TEMP_H_
#define MEASURE_TEMP_H_
/*!
@param uint8_t 1= Booster On, 0 = Booster Off

Supply booster - Changed PWM ratio
from 50% to slightly less to reduce
resultant DC voltage. Now ~5V with
3 volt battery voltage                                                           

*/

void boostSupply(int8_t);

/*!
@param void 

Use ADC to measure temperature from TMP37 
sensor - 20mV per degree C, 500mV at 25C.
Span's 0 to 1V equating to 0 to 50C
with each bit representing 1.07mV (~1mV).
                                                                      
*/


void measureTemp(void);

#endif /* MEASURE_TEMP_H_ */