#include <stdio.h>
#include <avr/io.h>
#include <compat/deprecated.h>
#include "uart.h"
#include <avr/interrupt.h>
int main(){
	int i;
	uart_init();
	sei();
	txend=0;
	for(i=0;i<10;i++){
		uart_putstart("I love you\n");
			while(!txend); txend=0;
		uart_putstart("You love me\n");
			while(!txend); txend=0;
		uart_putstart("He loves her.\n");
			while(!txend);
	}
	uart_putstart("End!!1\n");
	/*
		int count=0;
		for(i=3;i<=2000;i++){
		if(is_prime(i)) {
			printf("%d is a prime number !!!\n",i) ; 
			count++;
		}
		else	printf("%d is not a prime number !!!\n",i);	
	}
	printf("The end!!!\n prime count %d",count);
	*/
	while(1);
	return 0;
}
