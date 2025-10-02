#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include "uart.h"
void uart_init(void){
	UBRR0H=0x00;
	UBRR0L=0x07;
	sbi(UCSR0A,U2X0);
	sbi(UCSR0B,TXEN0);
}
void uart_putchar(char ch){
	if(ch=='\n'){
		uart_putchar('\r');
	}
	_delay_ms(50);
	UDR0 =ch;
}
