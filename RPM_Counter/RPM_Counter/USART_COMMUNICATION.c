/*
 * USART_COMMUNICATION.c
 *
 * Created: 28/08/2022 10:22:40 p. m.
 *  Author: Johan Lee
 */ 

#ifndef F_CPU
#define F_CPU 16000000
#endif

#define BAUD 1200						//Baudrate size
#define BRC ((F_CPU/(16*BAUD))-1)		//Baudrate configuration

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//Attach one "character" of 8 bits in uint8_t format 
void attach_Serial (uint8_t data){
	UDR0 = data;
	_delay_ms(30);
}

//Makes a line break and returns the carriage at the start
void line_Break_Serial (){
	attach_Serial(10);				//Break line
	attach_Serial(13);				//Carriage return
}

//Send a complete string through the usart protocol
void write_Serial (uint8_t *string){
	while (*string != NULL){
		attach_Serial(*string);
		string++;
	}
	line_Break_Serial();
}

//Initiate the UART protocol 
void init_USART (){
	UBRR0H = (BRC>>8);												//Baudrate configuration displaced 8 bits to the right
	UBRR0L = BRC;													//Baudrate configuration
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);					/*Receiver enable | Transmitter enable
																	  RX Complete Interrupt Enable*/
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);								//Word lenght of 8 bits
}

