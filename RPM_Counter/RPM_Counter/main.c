/*
 * RPM_Counter.c
 *
 * Created: 19/12/2022 07:03:31 p. m.
 * Author : Johan Lee
 */ 

//Define Cristal Frequency if is not defined
#ifndef F_CPU
#define F_CPU 16000000
#endif

/*//////////////////////////INCLUDES///////////////////////////////////*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdlib.h>
#include <LCD_SCREEN.h>


/*//////////////////////////DEFINES////////////////////////////////////*/
#define INTERRUPT_PIN PORTD2


/*//////////////////////////GLOBAL VARIABLES////////////////////////////////////*/
volatile uint16_t rev_cnt = 0;					//variables that are used in ISRs and normal function must be declared volatile
volatile uint16_t rpm;							//Specifically, the processor must reload the variable from SRAM whenever it is referenced.
volatile uint8_t value_ready_flag = 0;									


/*//////////////////////////ISRs////////////////////////////////////*/
///External Interrupts 0
ISR(INT0_vect){
	rev_cnt ++;					//Every time that the INT0 executes add 1
}

//Comp A Timer Interrupt 
ISR(TIMER1_COMPA_vect){
	rpm = rev_cnt * 15;			//The time interrupt every 4 sec, so multiply by 15 to have the rpm
	value_ready_flag = 1;		//The value is ready to update
	rev_cnt = 0;				//Clear the cnt
	TCNT1 =  0;					//Re-start the TIMER1
}


/*//////////////////////////FUNCTIONS////////////////////////////////////*/

void welcome_LCD(void){
	print_LCD((uint8_t *)"   Bienvenido");
	_delay_ms(1500);
	clear_LCD();
	print_LCD((uint8_t *)"Motor girando a");
}


void init_External_Interrupt0(void){
	DDRB &=		~(1<<INTERRUPT_PIN);		//Set the PORTD2 as Input
	EICRA |=	(1<<ISC01);					//Interrupt works at the falling edge
	EICRA &=	~(1<<ISC00);				
	EIMSK |=	(1<<INT0);					//External Interrupt Request 0 Enable
}


void init_Timer0(void){
	TCCR1A =	0X00;
	TCCR1B |=	(1<<CS02) | (1<<CS00);		//Set pre-scaler to 1024
	TIMSK1 |=	(1<<OCIE1A);				//Compare Match A Interrupt Enable
	OCR1A =		62500;						//Output Compare Register A set to 62500 to make an approximation every 4 sec
}


/*//////////////////////////MAIN/////////////////////////////////////*/
int main(void)
{
	char rpm_lcd [10];						//rpm will be converted to an array in this var
	init_LCD((uint8_t *)0x27);				//Initialize the LCD in PORTC
	welcome_LCD();
	init_External_Interrupt0();
	init_Timer0();
	sei();									//Set bit I from SREG (Enables global interrupts)
	
    while (1) 
    {
		if (value_ready_flag == 1){
			ATOMIC_BLOCK(ATOMIC_FORCEON){	//Enter in an Atomic Block, in order to disable the ISR
				itoa(rpm, rpm_lcd, 10);		//Store the rpm value as an array in rpm_lcd, so it can be displayed 
				value_ready_flag = 0;		//Rpm Value has been update so set to 0 the flag 
			}								
			gotoXY_LCD(2,0);
			print_LCD((uint8_t *)"                ");
			gotoXY_LCD(2, 0);
			print_LCD((uint8_t *)"     ");
			print_LCD((uint8_t *)rpm_lcd);
			print_LCD((uint8_t *)" rpm");
		}
		else{
		
		}
	}
}