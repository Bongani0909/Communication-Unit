/*
 * LCD_def.c
 *
 * Created: 2023/04/09 12:51:59
 *  Author: 219015150
 */ 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define rs PB2
#define en PB3
void lcdcmd(char cmdout)
{
	PORTB = cmdout;
	PORTB &= ~(1 << rs);
	PORTB |= (1 << en);
	_delay_ms(1);
	PORTB &= ~(1 << en);
}

void lcddata(char dataout)
{
	PORTB = dataout;
	PORTB |= (1 << rs);
	PORTB |= (1 << en);
	_delay_ms(1);
	PORTB &= ~(1 << en);
}


void dis_cmd(char cmd_value)
{
	char cmd_value1;

	cmd_value1 = cmd_value & 0xF0; // mask lower nibble because PA4-PA7 pins are used.
	lcdcmd(cmd_value1);            // send to LCD

	cmd_value1 = ((cmd_value << 4) & 0xF0); // shift 4-bit and mask
	lcdcmd(cmd_value1);                      // send to LCD
}

void dis_data(char data_value)
{
	char data_value1;

	data_value1 = data_value & 0xF0;
	lcddata(data_value1);

	data_value1 = ((data_value << 4) & 0xF0);
	lcddata(data_value1);
}

void LCD_String(char* data0)		/* Send string to LCD function */
{
	int i = 0;
	while (data0[i] != '\0')
	{
		dis_data(data0[i]);
		_delay_ms(100);
		i++;
	}
}

void lcd_init() // function for initialize
{
	DDRC = 0xFF;
	DDRB = 0xFF;
	_delay_ms(15);  // Wait for 15ms after power on
	dis_cmd(0x02); // to initialize LCD in 4-bit mode.
	dis_cmd(0x28); // to initialize LCD in 2 lines, 5X7 dots and 4bit mode.
	dis_cmd(0x0C);
	dis_cmd(0x06);
	dis_cmd(0x80); // Set cursor to beginning of first line
	char data0[] = "Wireless";
	char data1[] = "Communication";
	LCD_String(data0);
	dis_cmd(0xC0);
	LCD_String(data1);

}

void LCD_Clear()
{
	dis_cmd(0x01);		/* Clear display */
	_delay_ms(2);
	dis_cmd(0x80);		/* Cursor at home position */
}
