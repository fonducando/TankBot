/*
 * Tank Robot.cpp
 *
 * Created: 4/19/2016 11:05:36 AM
 * Author : Chris Calandro

 left direction D0
 right direction D4
 Ping D3
 Ping listenback interrupt D2
 Ping Servo B3 (OC2A)
 Motors sleep D6
 Step - D1
 Light sensor C0 (ADC0)
 LED strip D5
 */ 




#define F_CPU 8000000ul
#include <avr/io.h>
#include <util/delay.h>
#include<avr/interrupt.h>
#include "movements.h"
#include "robotping.h"
#include "lightsense.h"

void normalmovement()
{
	calculateforwarddistance();
	if (forwarddistance > 12)
	{
	  fullspeed();
      moveforward();
	}
	

	if (forwarddistance <= 12)
	{
		calculatealldistance();
		medspeed();
		switch (bestdirection)
		{
			case 1:
			moveforward();
			break;

			case 2:
			moveleft45();
			break;

			case 3:
			moveleft45();
			moveleft45();
			break;

			case 4:
			moveright45();
			break;

			case 5:
			moveright45();
			moveright45();
			break;

			case 6:
			movebackward();
			break;
		}
	}
}


int main(void)
{
    DDRD = 0b01111011;//1 is output
    DDRB = 0b00001001;
	DDRC = 0b00000000;
	motorssleep();
	initializetimer();
	startints();
	startpwm();
	servocenter;
	_delay_us(servohangtime);
	stoppwm();
	motorsawake();
	//motorssleep();
	
	while (1) 
    {

	lightonoff();
	normalmovement();
    }
}


