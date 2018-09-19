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
	/* 用来判断指针当前所指元素在小数点前还是小数点后 */ 
	int floatmode = 0;	
	double num = 0.0;
	while (*p != 0){
		if (*p != '.')
			if (floatmode)
				/* 处理小数点后的位数，将该指针所指向元素的后一个元素退位 */ 
				num += (*p-'0')/pow(10,floatmode++); 
			else
				/* 处理小数点前的位数，将该指针所指向的元素的前一个元素进位 */
				num = num * 10 + *p-'0';
		else
			floatmode = 1;
		p++;
	}
	return num;
}

/* 将读入的字符串去掉小数点后不影响数值大小的0 */
char *floatSimplify(char *str){
	char *p = str;
	while(*(++p)!=0);	/* 使指针指向字符串的结束标记(\0) */
	p--;				/* 使指针指向字符串的最后一个元素 */
	while(*p!=0){		/* 去掉小数点后不影响数值大小的0 */
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

/*实现2个复数的加法运算*/ 
complex *addCom(complex const *a, complex const *b){
	complex *ans = (complex *)malloc(sizeof(complex));
	ans->real = a->real + b->real;
	ans->imag = a->imag + b->imag;
	return ans;
}

/* 实现2个复数的乘法运算 */ 
complex *multiplyCom(complex const *a, complex const *b){
	complex *ans = (complex *)malloc(sizeof(complex));  /* 为复数分配相应的内存空间 */
	ans->real = a->real * b->real - a->imag * b->imag;
	ans->imag = a->real * b->imag + a->imag * b->real;
	return ans;
}

/* 实现2个复数的除法运算 */ 
complex *divideCom(complex const *a, complex const *b){
	DIV_ERR_MSG = 0;
	complex *ans = (complex *)malloc(sizeof(complex));
	if(b->imag == 0.0)
		if(b->real == 0.0){		/* 讨论当除数为0则无意义的情况 */
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

/* 输出运算结果 */ 
int printCom(complex const *src){
	char ans[30];
	sprintCom(ans, src);
	int len = printf("%s\n",ans);
	return len;
}

/* 对将要输出的运算结果进行预处理 */ 
int sprintCom(char *dest, complex const *src){
	int len = 0;
	char s_real[NUM_MAX] = {0};
	char s_imag[NUM_MAX] = {0};
	char temp[NUM_MAX+2] = {0};
	char ans[2*NUM_MAX+4] = {0};
	sprintf(s_real,"%lf",src->real);	/* 调用sprintf函数将运算结果的实部写入s_real */ 
	sprintf(s_imag,"%lf",src->imag);	/* 调用sprintf函数将运算结果的虚部写入s_imag */
	/*
	// Here is a bug of high digits.
	int i = 0;
	for(i = 0; i < NUM_MAX;i++)
		printf("%c\n",s_real[i]);
	*/
	/* 判断运算结果实部和虚部的情况并按照符合数学习惯的方式表示运算结果 */
	if (fabs(src->real-0)>1E-7){	/* 判断实部是否大于0 */ 
		len += sprintf(temp, "%s%s",floatSimplify(s_real),src->imag>0.0 ? "+":"");
		strcat(ans, temp);
	}
	if (fabs(src->imag-0)>1E-7){	/* 判断虚部是否大于0 */ 
		len += sprintf(temp, "%si",fabs(src->imag-1)<=1E-7 ? "" : fabs(src->imag+1)<=1E-7 ? "-" : floatSimplify(s_imag));
		strcat(ans, temp);
	}
	if (!len){						/* 若实部和虚部都等于0则直接输出0 */ 
		len += sprintf(temp, "0");
		strcat(ans, temp);
	}
	strcpy(dest, ans);
	return len;
}




