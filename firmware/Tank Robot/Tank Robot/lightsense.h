extern "C"
{
#include "light_ws2812.h"
#include "light_ws2812.c"
};


#define lightthreshold 65  //if sensor value is lower than this the light turns on
#define ledsize 6  // number of LEDs in the strip
#define ledvalue 175 // led brightness value


volatile uint8_t lightvalue; //returned from the sensor on ADC0
bool lighton; 


struct cRGB led[ledsize];


void readlightsensor()
{

ADMUX = 0b00100000; //read ADC0 1 time
ADCSRA |= 1<<ADEN ;
ADCSRA |= 1<<ADSC;
while (ADCSRA & (1<<ADSC))
{
//do nothing
}
lightvalue = ADCH;
ADCSRA |= 1<<ADIF;
ADCSRA &= (!(1<<ADEN));
}


void setlightswhite()
{
for (int i=0; i<ledsize; i++)
	{
	  led[i].r=ledvalue;led[i].g=ledvalue;led[i].b=ledvalue;
	  ws2812_setleds(led,ledsize);
	}
}


void setlightsoff()
{
for (int i=ledsize; i>=0; i--)
     {
	 	  led[i].r=0;led[i].g=0;led[i].b=0;
	 	  ws2812_setleds(led,ledsize);
	 }
}


void lightonoff()
{

readlightsensor();

if (lightvalue <= lightthreshold)
	{
	  if (lighton == 0)
		  {
			setlightswhite();
			lighton = 1;
		  }
	}

if (lightvalue > lightthreshold)
    {
	 if (lighton == 1)
	   {
	   setlightsoff();
	   lighton = 0;
	   }	
    }
	  
	
	
}