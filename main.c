

#include "icu.h"
#include "lcd.h"
#include "ultrasonic.h"
#include <avr/io.h>
#include<avr/interrupt.h>
int main(void)
{
	uint16 Distance = 0 ;
	SREG |= (1 << 7);
	LCD_init();
	Ultrasonic_init();
	LCD_displayString("Distance = ");
	LCD_displayStringRowColumn(0,14,"CM");

	while(1)
	{
		Distance = Ultrasonic_readDistance();
		LCD_moveCursor(0,11);
		LCD_intgerToString(Distance);
	}
}
