#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void delay (uint32_t k)	 // roughly 2 usec per loop at 8 MHz system clock
{
  volatile uint32_t x = k;
  while (x)  --x;
}

void pcmd(uint8_t in)
{
	uint8_t straight=0,back=0,left=0;
	if(!(in & 0x08))
		straight=1;
	
	if(!(in & 0x04))
		back=1;
	
	if (!(in & 0x02))
		left=1;
		
	if(straight)
	{
		if(!back)
			PORTB=0x14;
		else
			PORTB=0x0A;
	}
	else
	{
		if(left)
			PORTB=0x12;
		else
			PORTB=0x0C;
	}
	delay(500);
	PORTB=0;
}

int main(void)
{
	uint8_t input;
	// Initialize the RS232 communication link to the PC 19200, 8, 1, E
	UCSRB = _BV (RXEN) ;
  	UBRRH = 0;
  	UBRRL = 25;						
	// At 8MHz (12 =>38400) (25 => 19200)
  	UCSRC = _BV (URSEL) | _BV (UPM1) | _BV (UCSZ1) | _BV (UCSZ0); // 8,1,E
	DDRB = 0xFF;	
	while(1)
	{
		loop_until_bit_is_set (UCSRA, UDRE);
		input = UDR ;
		pcmd(input);
	}
}		
