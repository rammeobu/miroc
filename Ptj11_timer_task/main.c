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
	int tag;
	struct task task;
	uart_init();
	task_init();
	timer_init();
	printf("$ ");
	while(1){
		cli();
		tag=Task_delete(&task);
		sei();
		if(tag){
			(*(task.fun))(task.arg);
		}
	}
	return 0;
}
