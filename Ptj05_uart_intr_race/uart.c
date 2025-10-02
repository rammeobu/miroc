#include <stdio.h>
#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
void uart_init(void){
	UBRR0H=0x00;
	UBRR0L=0x07;
	UCSR0A |=(1<<U2X0);
	UCSR0B |=(1<<TXEN0);	
	UCSR0B |=(1<<TXCIE0);
}
char buf[64];
volatile int bufi;
volatile int txend=1;
void uart_putstart(char str[]){
	char ch;
	int n=strlen(str);
	while(sizeof(buf)-strlen(buf+bufi)-1<n);
	cli();
	strcpy(buf,buf+bufi);strcat(buf,str);
	bufi=0;
	if(txend){
		txend=0; ch=buf[bufi++];
		UDR0=ch;
	}
	sei();
}
int uart_putchar_1(char ch,FILE *stream){
	if(ch=='\n'){
		uart_putchar_1('\r',stream);
	}
	_delay_ms(100);
	UDR0 =ch;
	return (1);
}

int uart_putchar_2(char ch,FILE *stream){
	if(ch=='\n'){
		uart_putchar_2('\r',stream);
	}
	while(!(UCSR0A & (1<<UDRE0)) )//상태 체크
	;
	UDR0 =ch;
	return (1);
}
void uart_putchar(char ch){
	if(ch=='\n'){
		uart_putchar('\r');
	}
	_delay_ms(100);
	UDR0 =ch;
}
void uart_putstr(char *sp){
	for (;*sp;sp++){
		uart_putchar(*sp);
	}
}
int is_prime(int n){
		int i;
		for (i=2;i<=n/2;i++){
			if((n%i)==0)return(0);	
		}
		return(1);
}
ISR(USART0_TX_vect){
	char ch = buf[bufi];
	if(!ch){
		txend=1;
		return;
	}
	if(ch=='\n') buf[bufi]='\r';
	else bufi++;
	UDR0=ch;
}
 
