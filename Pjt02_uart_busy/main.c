#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include "led.h"
#include "uart.h"
int main(){
	char c;
	uart_init();
	for(int j =0;j<10;j++){
		for(c='A';c<='Z';c++)
			uart_putchar(c);
		uart_putchar('\n');
	}
	return 0;
}
