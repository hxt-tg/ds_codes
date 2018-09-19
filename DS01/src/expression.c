/* expression.c */

#include "expression.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

complex previous_ans;
/* const array */
char OP_SET[OP_LEN] = {'+','-','*','/','(',')','='};
char VALID_SET[VALID_LEN] = {'0','1','2','3','4','5','6','7','8','9',
							'.','+','-','*','/','(',')','i','p'};
int PRIORITY[OP_LEN][OP_LEN] = {
	{1,1,-1,-1,-1,1,1},
	{1,1,-1,-1,-1,1,1},
	{1,1,1,1,-1,1,1},
	{1,1,1,1,-1,1,1},
	{-1,-1,-1,-1,-1,0,-2},
	{1,1,1,1,-2,1,1},
	{-1,-1,-1,-1,-1,-2,0}
};

/* const array */
int Precede(op a, op b){
	int posa = 0;
	int posb = 0;
	while(a != OP_SET[posa]) posa++;
	while(b != OP_SET[posb]) posb++;
	return PRIORITY[posa][posb];
}

int InOP(op x){
	int pos = 0, i;
	for (i = 0; i < OP_LEN; i++)
		if (OP_SET[pos++] == x) return 1;
	return 0;
}

/* String Process Part */
Status checkExp(char *src){
	ERR_MSG err = RIGHT;
	char *p = src;
	/* Process end of '=' */
	while (*p++);
	p-=2;
	if(*p == '=')
		*p = 0;
	p = src;
	int pos = 0;
	int i;
	int chk_bracket = 0; /* check_bracket */
	enum {NUM_MODE, OP_MODE} chk_mode = OP_MODE; /* check_mode */
	int num_len = 0,first_num_pos; /* check number overflow */
	int chk_i = 0, chk_dot = 0, first_i_pos;
	while (p[pos] != 0){
		/* Check invalid character */
		int valid_char_found = 0;
		for(i = 0;i < VALID_LEN;i++)
			if(VALID_SET[i] == p[pos])
				valid_char_found = 1;
		if(!valid_char_found){
			err = INVALID_CHAR;
			break;
		}
		/* Definite error condition */
		if(InOP(p[pos])){
			// Enter operator mode
			chk_mode = OP_MODE;
			first_num_pos = pos;
			chk_i = 0;
			chk_dot = 0;
			// Check bracket and double operator
			if (p[pos] == ')')
				chk_bracket--;
			else{
				if (p[pos] == '('){
					/* (+   (*   (/ */
					if (InOP(p[pos+1])&&(p[pos+1]=='+'||p[pos+1]=='*'||p[pos+1]=='/')){
						pos++;
						err = LOST_OPND;
						break;
					}
					chk_bracket++;
				}
				else{
					if ((InOP(p[pos+1])||p[pos+1]==0)&&(p[pos+1]!='(')){
						/* [+,-,*,/][),+,-,*,/] */
						err = LOST_OPND;
						if (p[pos+1] == '-')
							err = LOST_OPND_MINUS;
						pos++;
						break;
					}
				}
			}
		}
		else{
			// Enter number mode
			/* Record first enter NUM_MODE position */
			if(chk_mode == OP_MODE)
				first_num_pos = pos;
			chk_mode = NUM_MODE;
			if (p[pos] == '.'){
				if (p[pos+1] == 'i'|| p[pos+1] == 'p' || InOP(p[pos+1])){
					/* .i	.<op> */
					err = UNEXPECTED_DOT;
					break;
				}
				chk_dot++;
			}
			// Record first i position to process <Excepted operator>	
			if (p[pos] == 'i'){
				chk_i++;
				if (chk_i == 1)
					first_i_pos = pos;
			}
			/* Check pp   p<num>    pi */
			if (p[pos] == 'p'){
				if((p[pos+1]>='0'&&p[pos+1]<='9')||p[pos+1]=='p'||p[pos+1]=='i'){
					err = INVALID_REUSE;
					break;
				}
			}
		}
		/* Error condition */
		/* ()) */
		if (chk_bracket < 0){
			
			err = LOST_OPEN_BRACKET;
			break;
		}
		/* ..	.<num>. */
		if (chk_dot > 1){
			err = INVALID_DOT;
			break;
		}
		/* ii	i<num>i */
		if (chk_i > 1){
			err = LOST_OPTR;
			pos = first_i_pos + 1;
			break;
		}
		/* Number overflow */
		if (pos - first_num_pos >= 10){
			err = NUM_OVERFLOW;
			pos = first_num_pos;
			break;
		}
	pos++;	
	}
	if (chk_bracket > 0)
		/* (() */
		err = LOST_CLOSE_BRACKET;
	return errorProcess(pos, err);
}

/* Pre-process module */
Status preProcess(char *src){
	// Process '\n'
	char *p = src;
	while (*p++);
	p--;
	if(*p == '\n')
		*p = 0;
	p = src;
	// Prepare a string to process input string
	char tp_str[EXP_MAX] = {0};
	int idx = 0;
	// First char is 'i' -> "1i"
	if(*p == 'i')
		tp_str[idx++] = '1';
	// First char is '-' -> "0-i"
	if(*p == '-')
		tp_str[idx++] = '0';
	// 'i' -> "1i"
	while (*p){
		tp_str[idx++] = *p;
		if (InOP(*p)){
			/*      ")(" -> ")*("  */
			/*  ")<num>" -> ")*<num>"  */
			/*      ")p" -> ")*p" */
			if(*p == ')'&&((*(p+1)>='0'&&*(p+1)<='9')||*(p+1)=='('||*(p+1)=='i'||*(p+1)=='p'))
				tp_str[idx++] = '*';
			/* "i" -> "1i" */
			if(*(p+1) == 'i')
				tp_str[idx++] = '1';
			/* "(-" -> "(0-)" */
			if(*p == '(' && *(p+1) == '-')
				tp_str[idx++] = '0';
		}
		/* "<num>(" -> "<num>*(" */
		/* "<num>p" -> "<num>*p" */
		if (((*p<='9'&&*p>='0')||*p=='i')&&(*(p+1)=='('||*(p+1)=='p'))
			tp_str[idx++] = '*';
		/* "p(" -> "p*(" */
		if (*p == 'p' && *(p+1) == '(')
			tp_str[idx++] = '*';
		p++;
	}
	tp_str[idx++] = '=';
	tp_str[idx++] = 0;
	strcpy(src,tp_str);
	// printf("[Transfrom]%s\n",src);
	return OK;
}

/* Calculate Part */
complex EvaluateExpression(char *src){
	// Prepare Stack
	opStack OPTR;
	comStack OPND;
	InitStack_op(&OPTR);
	Push_op(&OPTR,'=');
	InitStack_com(&OPND);
	// Init
	char *p = src;
	int idx = 0; // used to read numbers
	char theta, s_num[NUM_MAX];
	complex a, b, temp;
	while(*p!='=' || GetTop_op(&OPTR)!='='){
		if (!InOP(*p)){
			if(*p != 'i'){
				// push previous ans
				if (*p == 'p'){
					Push_com(&OPND,&previous_ans);
					p++;
					continue;
				}
				// get string of readable float
				idx = 0;
				do{
					s_num[idx] = *(p+idx);
					idx++;
				}while((*(p+idx) <='9' && *(p+idx)>='0')||*(p+idx) == '.');
				s_num[idx] = 0;
				p += idx;
				// Generate complex
				if (*p == 'i'){
					// Process imaginary part
					temp.imag = readOneNumber(s_num);
					temp.real = 0.0;
				}else{
					// Process real part
					temp.imag = 0.0;
					temp.real = readOneNumber(s_num);
				}
				Push_com(&OPND,&temp);
			}else{
				// skip character 'i'
				p++;
			}
		}else{
			switch(Precede(GetTop_op(&OPTR),*p)){
				case -1:	// lower priorty
					Push_op(&OPTR, *p);
					p++;
					break;
				case 0:		// Pop a open bracket
					Pop_op(&OPTR, &theta);
					p++;
					break;
				case 1:		// Pop and Evaluate, then Push again
					Pop_op(&OPTR, &theta);
					Pop_com(&OPND, &b);
					Pop_com(&OPND, &a);
					temp = Operate(&a, theta, &b);
					Push_com(&OPND, &temp);
					break;
			} //switch
		} //if
	} // while
	extern int DIV_ERR_MSG;
	if (DIV_ERR_MSG != 1) return previous_ans = GetTop_com(&OPND);
	else return GetTop_com(&OPND);
} // EvaluateExpression

complex Operate(complex *a, op x, complex *b){
	complex ans;
	switch (x){
		case '+':
			ans = *addCom(a,b);
			break;
		case '-':
			b->real *= -1;
			b->imag *= -1;
			ans = *addCom(a,b);
			break;
		case '*':
			ans = *multiplyCom(a,b);
			break;
		case '/':
			ans = *divideCom(a,b);
			break;
	}
	return ans;
}

