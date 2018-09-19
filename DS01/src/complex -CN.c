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
	/* �����ж�ָ�뵱ǰ��ָԪ����С����ǰ����С����� */ 
	int floatmode = 0;	
	double num = 0.0;
	while (*p != 0){
		if (*p != '.')
			if (floatmode)
				/* ����С������λ��������ָ����ָ��Ԫ�صĺ�һ��Ԫ����λ */ 
				num += (*p-'0')/pow(10,floatmode++); 
			else
				/* ����С����ǰ��λ��������ָ����ָ���Ԫ�ص�ǰһ��Ԫ�ؽ�λ */
				num = num * 10 + *p-'0';
		else
			floatmode = 1;
		p++;
	}
	return num;
}

/* ��������ַ���ȥ��С�����Ӱ����ֵ��С��0 */
char *floatSimplify(char *str){
	char *p = str;
	while(*(++p)!=0);	/* ʹָ��ָ���ַ����Ľ������(\0) */
	p--;				/* ʹָ��ָ���ַ��������һ��Ԫ�� */
	while(*p!=0){		/* ȥ��С�����Ӱ����ֵ��С��0 */
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

/*ʵ��2�������ļӷ�����*/ 
complex *addCom(complex const *a, complex const *b){
	complex *ans = (complex *)malloc(sizeof(complex));
	ans->real = a->real + b->real;
	ans->imag = a->imag + b->imag;
	return ans;
}

/* ʵ��2�������ĳ˷����� */ 
complex *multiplyCom(complex const *a, complex const *b){
	complex *ans = (complex *)malloc(sizeof(complex));  /* Ϊ����������Ӧ���ڴ�ռ� */
	ans->real = a->real * b->real - a->imag * b->imag;
	ans->imag = a->real * b->imag + a->imag * b->real;
	return ans;
}

/* ʵ��2�������ĳ������� */ 
complex *divideCom(complex const *a, complex const *b){
	DIV_ERR_MSG = 0;
	complex *ans = (complex *)malloc(sizeof(complex));
	if(b->imag == 0.0)
		if(b->real == 0.0){		/* ���۵�����Ϊ0������������ */
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

/* ��������� */ 
int printCom(complex const *src){
	char ans[30];
	sprintCom(ans, src);
	int len = printf("%s\n",ans);
	return len;
}

/* �Խ�Ҫ���������������Ԥ���� */ 
int sprintCom(char *dest, complex const *src){
	int len = 0;
	char s_real[NUM_MAX] = {0};
	char s_imag[NUM_MAX] = {0};
	char temp[NUM_MAX+2] = {0};
	char ans[2*NUM_MAX+4] = {0};
	sprintf(s_real,"%lf",src->real);	/* ����sprintf��������������ʵ��д��s_real */ 
	sprintf(s_imag,"%lf",src->imag);	/* ����sprintf���������������鲿д��s_imag */
	/*
	// Here is a bug of high digits.
	int i = 0;
	for(i = 0; i < NUM_MAX;i++)
		printf("%c\n",s_real[i]);
	*/
	/* �ж�������ʵ�����鲿����������շ�����ѧϰ�ߵķ�ʽ��ʾ������ */
	if (fabs(src->real-0)>1E-7){	/* �ж�ʵ���Ƿ����0 */ 
		len += sprintf(temp, "%s%s",floatSimplify(s_real),src->imag>0.0 ? "+":"");
		strcat(ans, temp);
	}
	if (fabs(src->imag-0)>1E-7){	/* �ж��鲿�Ƿ����0 */ 
		len += sprintf(temp, "%si",fabs(src->imag-1)<=1E-7 ? "" : fabs(src->imag+1)<=1E-7 ? "-" : floatSimplify(s_imag));
		strcat(ans, temp);
	}
	if (!len){						/* ��ʵ�����鲿������0��ֱ�����0 */ 
		len += sprintf(temp, "0");
		strcat(ans, temp);
	}
	strcpy(dest, ans);
	return len;
}




