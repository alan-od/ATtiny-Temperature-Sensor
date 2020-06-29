/*
some stuff

*/


/*!
 \mainpage
File: ATtiny48_FullSensor_v3
This version of the sensor application integrates the following functions:

@details

	SPI software
	nRF24L01 Transceiver interface software
	Battery voltage measuring software
	TMP37 temperature sensor software
	Power saving software

While integrating the power management software it was necessary to move the SPI initalisation
out from main and to the nRF24 module/function as the power management switches off the
SPI function and doesn't restart it until required.

*********re write this bit
This is version 3 of the application with the power management functions of v1 being re-coded.
When measuring processor current I noticed that current measurement value was unstable and I didn't
think the power management code was correct.  I wrote a separate program
(ATtiny48_LowPower_1) to try out the watchdog timer and the power management
built in functions.  Version 1 was not coded correctly and now only two functions are required,
wdtEnable() which effectivley sets up the sleep mode, and setupWDT() which enables the watchdog
timer to set up the appropriate delay (WDPx) plus sets up the interrupt  - note this does not
cause an interrupt/reset which the WDE does.  Note also that no code is required in the interrupt
handler.  Once these functions were coded, main was restructured to properly sequence the
various functions. The resultant changes gives a standby current of 30uA and when "I" was
measured the value was stable.

During testing I noticed the prototype would lock up if it were moved out of range of the
receiver. This lock up (I assume) was because the Tx would attempt to send a number of packets
but not receive an ack from the receiver.  The power management system would then kick in
closing down and restarting the SPI hardware and then the NRF24 would be accessed again.
This problem didn't appear to occur when the prototype was originally tested but then there was
no power management system. The solution was quite simple, I just added a reset in the txPayload() 
after readWrite(R,W_TX_PAYLOAD,writeBuf,5). While I was at it I tightened up the timing withing 
the code as there were too many delays() and I also reduced the value of the DELAY macro to 5mS.

One "Gotcha" that I came across concerns measuring voltage with the ADC.
It's important to read the High and Low ADC value registers in the correct order
otherwise the ADC conversion doesn't work - see code (and data sheet!).

@date
27/01/2020
@author
Author : alano

*/
