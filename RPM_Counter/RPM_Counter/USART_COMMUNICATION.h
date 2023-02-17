/*
 * USART_COMMUNICATION.h
 *
 * Created: 28/08/2022 10:26:24 p. m.
 *  Author: Johan Lee
 *		Implementation notes
 *		1 - This library was made to communicate to 1200 baudrates, 8 bits wordlenght, no parity, 1 stop bit
 *		2 - To change the baudrate, change the "BAUD" definition in the .c file at the top
 */ 


#ifndef USART_COMMUNICATION_H_
#define USART_COMMUNICATION_H_


void attach_Serial (uint8_t data);
/*Attach one "character" of 8 bits to the TX in uint8_t format 
  The parameter must be the "character" in uint8_t format*/



void line_Break_Serial ();
//Makes a line break and returns the carriage at the start



void write_Serial (uint8_t *string);
/*Send a complete string through the usart protocol
  The parameter must be a uint8_t string*/



void init_USART ();
//Initiate the UART protocol 

#endif /* USART_COMMUNICATION_H_ */