//main.c
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include <compat/deprecated.h>
#include "uart.h"
#include "uart_q.h"
#include "list.h"
#include "task.h"
#include "timer.h"
#include "adc.h"


extern void app_prime(char *ap);

int main()
{
	int tag;
	struct task task;

	uart_init();
	task_init();
	timer_init();
	adc_init();
	sei();
	printf("$ ");
	
	while(1){
		cli();
		tag = task_delete(&task);
		sei();
		if(tag) (*(task.fun))(task.arg);
	}
	return(0);		
}
