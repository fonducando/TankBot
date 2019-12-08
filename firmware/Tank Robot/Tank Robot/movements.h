#define leftforward (PORTD &= (~(1<<PORTD0)))
#define leftreverse (PORTD |= (1<<PORTD0))
#define rightforward (PORTD &= (~(1<<PORTD4)))
#define rightreverse (PORTD |= (1<<PORTD4))

int delaytime = 1000;

void motorssleep()
{
PORTD &= (~(1<<PORTD6));
_delay_ms(5);
}

void motorsawake() 
{
(PORTD |= 1<<PORTD6);
_delay_ms(5);
}

void sendsteps (uint16_t number)
{
	for(uint16_t i=0; i<number; i++)
	{
		PORTD |= 1<<PORTD1;
		PORTD &= (~(1<<PORTD1));

		for (uint16_t i=delaytime; i>0; i--)  // 0.2ms = 200us
		{
			_delay_us(1);
		}
	}
	
}

void moveright45 ()
{
	rightreverse; leftforward;
	sendsteps(750);
	rightforward; leftforward;
}

void moveleft45()
{
	rightforward; leftreverse;
	sendsteps(750);
	rightforward; leftforward;
}

void moveforward()
{
	rightforward; leftforward;
	sendsteps(100);
}

void movebackward()
{
 	rightreverse; leftreverse;
	sendsteps(1000);
	rightforward; leftforward;
}


void fullspeed()
{
delaytime = 500;
}

void medspeed()
{
delaytime = 1000;
}

void slowspeed()
{
delaytime = 2000;
}


