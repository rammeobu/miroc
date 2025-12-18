//uart.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <compat/deprecated.h>
#include <util/delay.h>
#include "uart.h"
#include "uart_q.h"
#include "task.h"
#define ESC 27

int uart_putchar(char ch, FILE *stream), uart_getchar(FILE *stream);

FILE Mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE Mystdin = FDEV_SETUP_STREAM (NULL, uart_getchar, _FDEV_SETUP_READ);
char uart_busy;
void task_cmd(char                                *arg);
static int echo_mode = 1;

void uart_init()
{
	stdin = &Mystdin;
	stdout = &Mystdout;
	q_init();
	uart_busy = 0;

	UBRR0H = 0x00; UBRR0L = 0x07;
	sbi(UCSR0A, U2X0);
	sbi(UCSR0B, TXEN0);
	sbi(UCSR0B, TXCIE0);
	sbi(UCSR0B, RXEN0);
	sbi(UCSR0B, RXCIE0);
}

ISR(USART0_TX_vect)
{

	char ch;

	if((ch = qo_delete()) == 0)
		uart_busy = 0;
	else
		UDR0 = ch;
}

ISR(USART0_RX_vect)
{
	struct task task;
	char ch;

	ch = UDR0;
	if (ch == ESC){
		echo_mode = (echo_mode +1) & 0x01;
		return;
	}

	if (ch != EOT)
	{
		if (ch == '\r')
		{
			ch = '\n';
		}
		if (echo_mode) uart_echo(ch);
	}
	qi_insert(ch);
	if(ch == EOT || ch == '\n'){
		task.fun = task_cmd;
		strcpy(task.arg, "");
		task_insert(&task);
	}
}

int uart_putchar(char ch, FILE *stream)
{
	if (ch == '\n')
		uart_putchar('\r', stream);

	cli();

	if (!uart_busy) 
	{
		UDR0 = ch;
		uart_busy = 1;
	}
	else 
	{
		while(qo_insert(ch) == 0) 
		{
			sei();
			_delay_us(100);
			cli();
		}
	}
	sei();

	return (1);
}

int uart_getchar(FILE *stream)
{
	char ch;

	do
	{
		cli();
		ch = qi_delete();
		sei();
	} while (ch == 0);
	
	if (ch == EOT) return(-1);
	else return(ch);
}

void uart_echo(char ch)
{
	if (ch == '\n')
	{
		uart_echo('\r');
	}
	if (!uart_busy)
	{
		UDR0 = ch;
		uart_busy = 1;
	}
	else
		qo_insert(ch);
	
}

int is_prime(int n)
{
	int i;
	for(i = 2; i <= n/2 ; i++)
		if((n % i) == 0)
			return (0);
	return (1);
}
