//adc.c
#include <stdio.h>
#include <avr/io.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "task.h"
#include "uart.h"
#include "timer.h"

void adc_init()
{
	cbi(DDRF, PF0); // make input PF0(=ADC0) GPIO
	ADMUX = 0x80 | 0x00; // Ref. : 1.1v, Port : PF0
}

void adc_start()
{
	sbi(ADCSRA, ADEN); // ADC enable
	_delay_us(120);
	sbi(ADCSRA, ADIE); // Interrupt Enable
	sbi(ADCSRA, ADSC); // Start conversion
}

ISR(ADC_vect)
{
   	uint16_t data;
    char arg[8];
    
    data = ADCW & 0x03ff;
    cbi(ADCSRA, ADEN);
    
    sprintf(arg, "%d", data);
    task_tc1047(arg);
}
