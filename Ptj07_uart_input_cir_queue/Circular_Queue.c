#include <stdio.h>
#define Q_SIZE 64
#define QO_SIZE 64
#define QI_SIZE 64
char qi[QI_SIZE],qo[QO_SIZE];
char q[Q_SIZE];
int f,r;
int fi,ri,fo,ro;
void q_init(){
	fi=ri=fo=ro=0;
}
void qi_init(){
	fi=ri=fo=ro=0;
}
int q_insert(char ch){
	if((r+1)%Q_SIZE==f){
		return(0);
	}
	r=(r+1)%Q_SIZE;
	q[r]=ch;
	return(1);
	
}
int q_delete(){
	if(r==f)
		return 0;
	f=(f+1)%Q_SIZE;
	return(q[f]);
}
int ql_insert(char ch){
	if((ri+1)%QI_SIZE==fi){
		return(0);
	}
	ri=(ri+1)%QI_SIZE;
	qi[ri]=ch;
	return(1);
	
}
int qo_insert(char ch){
	if((ro+1)%QO_SIZE==fo){
		return(0);
	}
	ro=(ro+1)%QO_SIZE;
	qo[ro]=ch;
	return(1);
	
}
int ql_delete(){
	if(ri==fi)
		return 0;
	fi=(fi+1)%QI_SIZE;
	return(qi[fi]);
}
int qo_delete(){
	if(ro==fo)
		return 0;
	fo=(fo+1)%QO_SIZE;
	return(qo[fo]);
}
