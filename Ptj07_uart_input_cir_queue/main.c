#include <stdio.h>
#include <avr/io.h>
#include <compat/deprecated.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include "Circular_Queue.h"
int main(){
	char cmd[128],ch;
	int n=0;
	uart_init();
	sei();
	printf("$ ");
	while(1){
		cli();
		ch=ql_delete();
		sei();
		if(ch){
			if(ch=='\r')continue;
			if(ch=='\n'){
				cmd[n]=0;
				if(!strcmp(cmd,"app")) app_prime(2000);
				else 	printf("Unknon command..\n");
				n=0;
				printf("$ ");
			}else{
				cmd[n++]=ch;
			}

		}
	}
	
	return 0;
}
