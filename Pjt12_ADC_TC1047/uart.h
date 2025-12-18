//uart.h
#include<stdio.h>
#define EOT 0x04

extern char uart_busy;
void task_cmd(char *arg);

void uart_init(void);
int uart_putchar(char ch, FILE *stream);//stdio.hø°¿÷¿Ω
int uart_getchar(FILE *stream);
void uart_echo(char ch);
int is_prime(int n);
