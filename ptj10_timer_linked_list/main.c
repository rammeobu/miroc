#include <avr/io.h>
#include <compat/deprecated.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "time_linked_list.h"
#include "uart.h"
#include "Circular_Queue.h"
#include "prime.h"
#include "linked_list.h"

int main(){
	char cmd[128],*cp,*ap;
	uart_init();
	sei();
	while(1){
		printf("$ ");
		if(fgets(cmd, sizeof(cmd),stdin)==NULL) break;
		if((cp=strtok(cmd," \n\r\t"))==NULL) continue;
		ap = strtok(NULL," \n\r\t");
		if (!strcmp(cp,"prime")){ 
			app_prime(ap);
		}else if (!strcmp(cp,"list")){
			app_list(ap);
		}else if(!strcmp(cp,"timer")){
			app_timer(ap);
		}else {
			printf("not command\n");
		}
	}
	printf("logout, good bye !!!\n");
	while(1); return 0;
}
