#include <stdio.h>
#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "Circular_Queue.h"
#include "uart.h"
#define EOT 0x04
FILE Mystdout = FDEV_SETUP_STREAM(uart_putchar,NULL,_FDEV_SETUP_WRITE);
FILE Mystdin = FDEV_SETUP_STREAM(NULL,uart_getchar,_FDEV_SETUP_READ);
char uart_busy;
void uart_init(void){
	stdout=&Mystdout;
	stdin=&Mystdin;
	qi_init();
	uart_busy=0;
	UBRR0H=0x00;UBRR0L=0x07;
	sbi(UCSR0A,U2X0);
	sbi(UCSR0B,TXEN0);
	sbi(UCSR0B,TXCIE0);
	sbi(UCSR0B,RXEN0);
	sbi(UCSR0B,RXCIE0);
}
int uart_putchar(char ch, FILE * stream){
	if (ch=='\n')uart_putchar('\r',stream);
	cli();
	if(!uart_busy){
		UDR0=ch;
		uart_busy=1;
	}
	else{
		while(qo_insert(ch)==0){
			sei();
			_delay_us(100);
			cli();
		}
	}
	sei();
	return (1);
}
int uart_getchar(FILE * stream){
	char ch;
	do{
		cli();
		ch=qi_delete();
		sei();
	}while (ch==0);
	if (ch==EOF)return (1);
	else return (ch);
}

void uart_echo(char ch){
	if(ch=='\n')uart_echo('\n');
	if(!uart_busy){
		UDR0=ch;
		uart_busy=1;
	}else{
		qo_insert(ch);
	}
}
ISR(USART0_TX_vect){
	char ch ;
	if((ch=qo_delete())==0)
		uart_busy=0;
	else
		UDR0=ch;	
}

ISR(USART0_RX_vect){
	char ch;
	ch=UDR0;
	if(ch!=EOF){
		if(ch=='\n')ch='\n';
		uart_echo(ch);
	}	
	qi_insert(ch);
}

 
