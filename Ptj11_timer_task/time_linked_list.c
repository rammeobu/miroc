#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <compat/deprecated.h>

#include "time_linked_list.h"
#include "prime.h"
#include "Circular_Queue.h"
#include "uart.h"
#include "linked_list.h"

#define MAX_TASK 16
#define ESC 0x1b
#define EOT 0x04

static int echo_mode =1;
struct timer *Thead=NULL;

struct task Task_q[MAX_TASK];
volatile int Task_f,Task_r;

void timer_init(){
	TCNT2=0;
	sbi(ASSR,AS2);
	sbi(TIMSK2,TOIE2);
	sbi(TCCR2B,CS20);
	sbi(TCCR2B,CS21);
}
void task_init(){
	Task_f=Task_r=0;
}
int task_insert(struct task *tskp){
	if((Task_r+1)%MAX_TASK==Task_f){
		return(0);
	}
	Task_r=(Task_r+1)%MAX_TASK;
	Task_q[Task_r]=*tskp;
	return(1);
	
}
int Task_delete(struct task *tskp){
	if(Task_r==Task_f)
		return 0;
	Task_f=(Task_f+1)%MAX_TASK;
	*tskp=Task_q[Task_f];
	return(1);
}
struct timer *get_timer(){
	struct timer *tp;
	tp=(struct timer *)malloc(sizeof(*tp));
	return(tp);	
}
void insert_timer(struct task *tskp,int ms){
	int total;
	struct timer *tp,*cp,*pp;
	tp=get_timer();
	tp->task=*tskp;
	tp->time=ms;
	if(Thead == NULL){
		Thead = tp,tp->link=NULL;
		return;
	}
	pp=NULL,total=0;
	for(cp=Thead;cp;cp=cp->link){
		total+= cp->time;
		if(total>=ms){
			break;
		}
		pp=cp;

	}
	if(pp==NULL){
		cp->time-=tp->time;
		tp->link=cp,Thead=tp;
	}else if(cp==NULL){
		tp->time -= total;
		pp->link = tp;
		tp->link=NULL;

	}else{
		total-= cp->time;
		tp->time -=total;
		cp->time -= tp->time;
		pp->link=tp;
		tp->link=cp;
	}
}
void tour_timer(){
	struct timer *cp;
	int total=0;
	printf("\n");
	for(cp=Thead; cp!=NULL; cp = cp->link){
		total+=cp->time;
		printf("-->%d(%d) ", cp->time,total);
	}
	printf("\n");
}
void free_timer(){
	struct timer *cp;
	for (; Thead != NULL;){
		cp = Thead;
		Thead = cp->link;
		free(cp);
	}
}
void app_timer(char *ap){
	char buf[16];
	int ms;
	struct task tsk;
	while (1){
		printf(">> ");
		if(fgets(buf,16,stdin)==NULL || (ms= atoi(buf))==0){
			break;
		}
		insert_timer(&tsk,ms);
	}
	tour_timer();
	free_timer();
}
void timer_expire(void){
	struct timer *tp;
	for( ;Thead !=NULL && !Thead->time; ){
		tp = Thead,Thead=tp->link;
		task_insert(&tp->task);
		free(tp);
	}
}
void task_cmd(char *arg){
	char buf[64], *cp0,*cp1,*cp2,*cp3;
	struct task task;
	int ms;
	if(!uart_peek()) return;
	if(gets(buf)==NULL){
		printf("logout\n");
		while(1);	
	}
	cp0=strtok(buf," \n\r\t");
	cp1=strtok(NULL," \n\r\t");
	cp2=strtok(NULL," \n\r\t");
	cp3=strtok(NULL," \n\r\t");
	if(cp0==NULL){
		printf("$ ");
		printf("!!!-111\n");
		return;
	}
	if(!strcmp(cp0, "prime")){
		task_prime(cp1);
	}else if(!strcmp(cp0,"list")){
		app_list(cp1);
	}else if(!strcmp(cp0,"timer")){
		if(cp1==NULL){
			printf("!!!-222\n");
			printf("$ ");
			return;
		}
		ms=atoi(cp1)/256;
		if(!strcmp(cp2,"prime")){
			task.fun=task_prime;
			if(cp3) strcpy(task.arg, cp3);
			else strcpy(task.arg,"");
			cli();
			insert_timer(&task,ms);
			sei();
				
		}else if(!strcmp(cp2,"list")){
			task.fun=app_list;
			if(cp3){
				 strcpy(task.arg, cp3);
			}else{
				 strcpy(task.arg,"");
				cli();
				insert_timer(&task,ms);
				sei();
			}
		}
		else printf("!!!-333\n");
	}
	else printf("!!!-eree\n");
	printf("$ ");
}

ISR(TIMER2_OVF_vect){
	if(!Thead)
		return;
	if(--Thead->time==0)
		timer_expire();
}
ISR(USART0_RX_vect)
{
	struct task task;
	char ch;
	ch = UDR0;
	if(ch == ESC) {
		echo_mode=(echo_mode+1)&0x01;
		return;
	}
	if(ch !=EOT){
		if(ch=='\r') ch='\n';
		if(echo_mode) uart_echo(ch);
	}
	qi_insert(ch);
	if(ch==EOT || ch== '\n'){
		task.fun=task_cmd;
		strcpy(task.arg,"");
		task_insert(&task);
	}
}
