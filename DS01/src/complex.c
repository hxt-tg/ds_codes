/* complex.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "complex.h"
#include "error.h"
#define NUM_MAX 15

/* Read a number from a string */
double readOneNumber(char *src){
	char *p = src;
	/* Locate pointer with decimal dot */ 
	int floatmode = 0;	
	double num = 0.0;
	while (*p != 0){
		if (*p != '.')
			if (floatmode)
				/* Process digits after decimal dot, move digits toward lower digit */ 
				num += (*p-'0')/pow(10,floatmode++); 
			else
				/* Process digits before decimal dot, move digits toward higher digit */ 
				num = num * 10 + *p-'0';
		else
			floatmode = 1;
		p++;
	}
	return num;
}

/* Process the redundant 0 at the end of a float */
char *floatSimplify(char *str){
	char *p = str;
	while(*(++p)!=0);	/* Move pointer to \0 */
	p--;				/* Move pointer to the last character */
	while(*p!=0){
		if(*p=='0')
			*p = 0;
		else{
			if(*p=='.')
				*p = 0;
			break;
		}
		p--;
	}
	return str;
}

/* Add 2 complex numbers and return the pointer of result */ 
complex *addCom(complex const *a, complex const *b){
	complex *ans = (complex *)malloc(sizeof(complex));
	ans->real = a->real + b->real;
	ans->imag = a->imag + b->imag;
	return ans;
}

/* Multiply 2 complex numbers and return the pointer of result */
complex *multiplyCom(complex const *a, complex const *b){
	complex *ans = (complex *)malloc(sizeof(complex));  /* 为复数分配相应的内存空间 */
	ans->real = a->real * b->real - a->imag * b->imag;
	ans->imag = a->real * b->imag + a->imag * b->real;
	return ans;
}

/* Divide a complex number with another one and return the pointer of result */
complex *divideCom(complex const *a, complex const *b){
	DIV_ERR_MSG = 0;
	complex *ans = (complex *)malloc(sizeof(complex));
	if(b->imag == 0.0)
		if(b->real == 0.0){		/* [Err]Devidend is 0. */
			errorProcess(-1,DIV_ZERO);
			DIV_ERR_MSG = 1;
			ans->real = 0.0;
			ans->imag = 0.0;
		}
		else{
			ans->real = a->real / b->real;
			ans->imag = a->imag / b->real;
		}
	else{
		double k = a->imag/b->imag;
		ans->real = k + (a->real*b->real-k*b->real*b->real)/(b->real*b->real+b->imag*b->imag);
		ans->imag = (a->imag*b->real-a->real*b->imag)/(b->real*b->real+b->imag*b->imag);
	}
	return ans;
}

/* Print result to screen */ 
int printCom(complex const *src){
	char ans[30];
	sprintCom(ans, src);
	int len = printf("%s\n",ans);
	return len;
}

/* Print result to a string */ 
int sprintCom(char *dest, complex const *src){
	int len = 0;
	char s_real[NUM_MAX] = {0};
	char s_imag[NUM_MAX] = {0};
	char temp[NUM_MAX+2] = {0};
	char ans[2*NUM_MAX+4] = {0};
	sprintf(s_real,"%lf",src->real);	/* Print real part to s_real */ 
	sprintf(s_imag,"%lf",src->imag);	/* Print imaginary part to s_imag */
	/*
	// Here is a bug of high digits.
	int i = 0;
	for(i = 0; i < NUM_MAX;i++)
		printf("%c\n",s_real[i]);
	*/
	/* Print result with regular format */
	if (fabs(src->real-0)>1E-7){	/* real part > 0 ? */ 
		len += sprintf(temp, "%s%s",floatSimplify(s_real),src->imag>0.0 ? "+":"");
		strcat(ans, temp);
	}
	if (fabs(src->imag-0)>1E-7){	/* imaginary part > 0 ? */ 
		len += sprintf(temp, "%si",fabs(src->imag-1)<=1E-7 ? "" : fabs(src->imag+1)<=1E-7 ? "-" : floatSimplify(s_imag));
		strcat(ans, temp);
	}
	if (!len){						/* print 0 when real part and imaginary part is 0 */ 
		len += sprintf(temp, "0");
		strcat(ans, temp);
	}
	strcpy(dest, ans);
	return len;
}




