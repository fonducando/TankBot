
#define pingport PORTD
#define pingpin PORTD2
volatile uint64_t timer;
volatile float time;
volatile double distance ;
volatile double forwarddistance = 0;
volatile double left45distance = 0;
volatile double left90distance = 0;
volatile double right45distance = 0;
volatile double right90distance = 0;
volatile int bestdirection = 1; //1=forward 2=left45 3=left90 4=right45 5=left90 6=backwards 
volatile int servolocation = 0; //1=forward 2=left45 3=left90 4=right45 5=left90 6=backwards 
static double servohangtime = 650; // the amount of time the servo waits before moving again

#define servoleft90 (OCR2A = 50)
#define servocenter (OCR2A = 110)
#define servoleft45 (OCR2A = 75)
#define servoright45 (OCR2A = 135)
#define servoright90 (OCR2A = 165)




void initializetimer ()  // setup pwm 31250
{
    //timer for distance calc
	TCCR0B |= 1<<CS00;
	TIMSK0 |= 1<<TOIE0;  

}

void startpwm()
{
	//phase correct
    TCCR2A |= 1<<COM2A1;
    TCCR2A |= 1<<WGM20;
    //Prescaller /64   
	TCCR2B |= 1<<CS22;
}

void stoppwm()
{
	//phase correct
	TCCR2A &= (~(1<<COM2A1));
	TCCR2A &= (~(1<<WGM20));
	//Prescaller /64
	TCCR2B &= (~(1<<CS22));
}

void stopints ()
{
	EICRA &= (~(1<<ISC00));  //stop int0
	EICRA &= (~(1<<ISC01));
	EIMSK &= (~(1<<INT0));
}

void startints ()
{
	EICRA |= 1<<ISC00;
	EICRA |= 1<<ISC01; //rising edge on int0
	EIMSK |=1<<INT0;  //start int0
	sei();
}

void calculateforwarddistance()
{  
	if (servolocation != 1)
		{
		startpwm();
		servocenter;
		_delay_ms(servohangtime);
		servolocation = 1;
		stoppwm();
		}
	
	stopints(); //stop int0
	DDRD |= 1<<DDD2; //set D2 as output
	pingport |= 1<<pingpin;  //ping high
	_delay_us(10);         //send for 10 microseconds
	pingport &= (~(1<<pingpin)); // ping low
	DDRD &= (~(1<<DDD2)); //set D2 as input
	startints(); //start int0 rising edge
	_delay_us(250);
	forwarddistance = distance;
	//motorsawake();
}

void calculateleft45distance()
{
    //motorssleep();

		if (servolocation != 2)
			{
			startpwm();
			servoleft45;
			_delay_ms(servohangtime);
			servolocation = 2;
			stoppwm();
			}

		stopints(); //stop int0
		DDRD |= 1<<DDD2; //set D2 as output
		pingport |= 1<<pingpin;  //ping high
		_delay_us(10);         //send for 10 microseconds
		pingport &= (~(1<<pingpin)); // ping low
		DDRD &= (~(1<<DDD2)); //set D2 as input
		startints(); //start int0 rising edge
		_delay_us(250);
		left45distance = distance;
		//motorsawake();
}

void calulateleft90distance()
{
    //motorssleep();

	if (servolocation != 3)
		{
		startpwm();
		servoleft90;
		_delay_ms(servohangtime);
		servolocation = 3;
		stoppwm();
		}

	stopints(); //stop int0
	DDRD |= 1<<DDD2; //set D2 as output
	pingport |= 1<<pingpin;  //ping high
	_delay_us(10);         //send for 10 microseconds
	pingport &= (~(1<<pingpin)); // ping low
	DDRD &= (~(1<<DDD2)); //set D2 as input
	startints(); //start int0 rising edge
	_delay_us(250);
	left90distance = distance;
	//motorsawake();
}

void calculateright45distance()
{
    //motorssleep();
	if (servolocation != 4)
		{
		startpwm();
		servoright45;
		_delay_ms(servohangtime);
		servolocation = 4;
		stoppwm();
		}


	stopints(); //stop int0
	DDRD |= 1<<DDD2; //set D2 as output
	pingport |= 1<<pingpin;  //ping high
	_delay_us(10);         //send for 10 microseconds
	pingport &= (~(1<<pingpin)); // ping low
	DDRD &= (~(1<<DDD2)); //set D2 as input
	startints(); //start int0 rising edge
	_delay_us(250);
	right45distance = distance;
	//motorsawake();
}

void calculateright90distance()
{
    //motorssleep();

	if (servolocation != 5)
		{
		startpwm();
		servoright90;
		_delay_ms(servohangtime);
		servolocation = 5;
		stoppwm();
		}

	stopints(); //stop int0
	DDRD |= 1<<DDD2; //set D2 as output
	pingport |= 1<<pingpin;  //ping high
	_delay_us(10);         //send for 10 microseconds
	pingport &= (~(1<<pingpin)); // ping low
	DDRD &= (~(1<<DDD2)); //set D2 as input
	startints(); //start int0 rising edge
	_delay_us(250);
	right90distance = distance;
	//motorsawake();
}

void calculatealldistance()     //between the ping and the setup interrupts, this should calculate distance in inches.
{
calculateforwarddistance();
if (forwarddistance >=1)
	{
	bestdirection = 1;
	}
calculateleft45distance();
if (left45distance > forwarddistance)
	{
	bestdirection = 2;
	}
calulateleft90distance();
	if (left90distance > left45distance)
	{
	bestdirection = 3;
	}
calculateright45distance();
	if (right45distance > left90distance)
	{
	bestdirection = 4;
	}
calculateright90distance();
	if (right90distance > right45distance)
	{
	bestdirection = 5;
	}
	//if (right90distance <= 6)
	//{
	//bestdirection = 6;
	//}

startpwm();
servocenter;
servolocation = 1;
_delay_ms(400);
stoppwm();
}

//void calculatedistance()
//{
	//stopints();  //stop int0
	//DDRD |= 1<<DDD2;
	//pingport |= 1<<pingpin;  //ping high
	//_delay_us(10);         //send for 10 microseconds
	//pingport &= (~(1<<pingpin)); // ping low
	//DDRD &= (~(1<<DD2));
	//startints();  //start int0
	//_delay_ms(5);
//}



ISR (INT0_vect)
{
	timer = 0;
	sei();
	while (PIND & (1<<PIND2))
	{
		//forwarddistance = 0;  //do nothing while we wait for the response to finish.  The timer should be counting
	}
	time = timer *.00451 / 2;  // calculate time using the timer
	distance = ((time * 1125)/12);

}

ISR (TIMER0_OVF_vect) //the timer overflow interrupt adds 1 to the timer and then is cleared by the overflow interrupt enable
{
	timer ++;
}
