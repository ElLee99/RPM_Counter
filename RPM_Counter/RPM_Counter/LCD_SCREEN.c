/*
 * LCD_SCREEN.c
 *
 * Created: 01/08/2022 04:27:52 p. m.
 *  Author: Johan Lee
 */ 

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "LCD_SCREEN.h"


uint8_t *DDR;
uint8_t *PORT;
uint8_t *PIN;


uint8_t bitmask_Less4_Significant (uint8_t value);
uint8_t bitmask_Most4_Significant_Shifted (uint8_t value);
void write_LCD_8bits (uint8_t action, uint8_t data);


//Returns the 4 less significants bits from a uint8_t value
uint8_t bitmask_Less4_Significant (uint8_t value){
	uint8_t mask = 0x0F;
	value = value & mask;
	return value;
}


//Returns the 4 most significants bits from a uint8_t value shifted in the 4 less significants bits
uint8_t bitmask_Most4_Significant_Shifted (uint8_t value){
	uint8_t mask = 0xF0;
	value = value & mask;
	value = value >> 4;
	return value;
}


//Does a enable function to write control commands or write on the screen of the LCD 
void enable_LCD (void){
	*PORT &= ~(1<<4);			//Turns off the enable pin
	_delay_ms(1);
	*PORT |= (1<<4);			//Turns on the enable pin
	_delay_ms(1);
	*PORT &= ~(1<<4);			//Turns off the enable pin
	_delay_ms(10);
}


/*Writes a command or character on the LCD using 4 bits configuration
  The parameters must be: 
							 action: 1 for control commands
									 2 for characters on the screen	
							 data: the value of the command control or the ASCII value of the character	
*/
void write_LCD (uint8_t action, uint8_t data){
	uint8_t register_Select = 0;
	
	if (action == 2){
		register_Select |= (1<<5);
	}

	*PORT = bitmask_Most4_Significant_Shifted(data)+ register_Select;
	enable_LCD();
	*PORT = bitmask_Less4_Significant(data) + register_Select;
	enable_LCD();
}


/*Writes a command or character on the LCD using 8 bits configuration, this is ONLY used in the
  init_LCD(uint8_t) function 
  The parameters must be:
							action: 1 for control commands
									2 for characters on the screen
							data: the value of the command control or the ASCII value of the character
  */
void write_LCD_8bits (uint8_t action, uint8_t data){
	uint8_t register_Select = 0;

	if (action == 2){
		register_Select |= (1<<5);
	}
	
	*PORT = bitmask_Most4_Significant_Shifted(data) + register_Select;
	enable_LCD();
}


//Clear the LCD
void clear_LCD (void){
	write_LCD(1, 0x01);
}


//Returns the cursor to home position
void return_Home_LCD (void){
	write_LCD(1, 0x02);
}


/*Initializes the LCD screen when it begins the program
	The parameter must be the DDR direction from one port
	0x24 set the PORTB
	0x27 set the PORTC
	0x2A set the PORTD		
*/
void init_LCD(uint8_t *dir_DDRX){			
	
	DDR = dir_DDRX;
	
	switch ((uint8_t)dir_DDRX){
											//Set the PORTB to be used
		case 0x24:						
			PIN = (uint8_t*) 0x23;
			PORT = (uint8_t*) 0x25;
			break;
											//Set the PORTC to be used
		case 0x27:
			PIN = (uint8_t*) 0x26;
			PORT = (uint8_t*) 0x28;
			break;
											//Set the PORTD to be used
		case 0x2A:
			PIN = (uint8_t*) 0x29;
			PORT = (uint8_t*) 0x2B;
			break;
		
		default:
			exit(EXIT_FAILURE);
			break;
	}

											//Initializes the LCD
	*DDR = 0x3F;
	_delay_ms(50);
	write_LCD_8bits(1, 0x30);
	write_LCD_8bits(1, 0x30);
	write_LCD_8bits(1, 0x30);
	write_LCD_8bits(1, 0x20);
	write_LCD(1, 0x28);
	write_LCD(1, 0x0F);
	clear_LCD();
	write_LCD(1, 0x06);
	
}



/*The LCD cursor moves to a specified position
  The parameters must be:
							 row: 1 to write in the first row
								  2 to write in the second row
							 col: 1-16 to write in the selected column 
*/
void gotoXY_LCD (uint8_t row, uint8_t col){
	uint16_t coordinate;
	if (row == 1){
		coordinate = 0x7F + col;
	}
	else{
		coordinate = 0xBF + col;
	}
	write_LCD(1, coordinate);
}



/*Writes a whole phrase on the LCD until gets a NULL
  The parameter must be a string
*/
void print_LCD(uint8_t *string){
	while (*string != NULL){
		write_LCD(2, *string);
		string ++;
	}
}