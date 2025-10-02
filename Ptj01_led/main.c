#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
int main(){
	led_init();
	while(1){
		led_init();
			led_off(1);
			led_off(2);
			led_off(3);
			led_off(4);
			_delay_ms(1000);	
	}
	return 0;
}
