/*
 * LCD_SCREEN.h
 *
 * Created: 04/08/2022 08:15:06 p. m.
 *  Author: Johan Lee
  *		Implementation notes
  *		1 - This library was made to be used in 4 bits LCD configuration and to be used with the atmega328p micro-controller
  *		2 - There are needed 6 bits from a port (THE SAME PORT), but it is recommended to use a entire port
  *		3 - The connection to the LCD must be done as it follows
  *			PORTX = x, x, RS, E, D7, D6, D5, D4
 */ 


#ifndef LCD_SCREEN_H_
#define LCD_SCREEN_H_


void enable_LCD (void);
//Does a enable function to write control commands or write on the screen of the LCD 



void write_LCD (uint8_t action, uint8_t data);
/*Writes a command or character on the LCD using 4 bits configuration
  The parameters must be: 
							 action: 1 for control commands
									 2 for characters on the screen	
							 data: the value of the command control or the ASCII value of the character	
*/



void clear_LCD (void);
//Clear the LCD



void return_Home_LCD (void);
//Returns the cursor to home position



void init_LCD(uint8_t *dir_DDRX);
/*Initializes the LCD screen when it begins the program
	The parameter must be the DDR direction from one port
	0x24 set the PORTB
	0x27 set the PORTC
	0x2A set the PORTD		
*/



void gotoXY_LCD (uint8_t row, uint8_t col);
/*The LCD cursor moves to a specified position
  The parameters must be:
							 row: 1 to write in the first row
								  2 to write in the second row
							 col: 1-16 to write in the selected column 
*/



void print_LCD(uint8_t *string);
/*Writes a whole phrase on the LCD until gets a NULL
  The parameter must be a string
*/


#endif /* LCD_SCREEN_H_ */