/* stack.c */

#include "stack.h"
#include <stdlib.h>

Status InitStack_op(opStack *s){
	s->base = (op *)malloc(STACK_INIT_SIZE * sizeof(op));
	if (!s->base) exit(OVERFLOW);
	s->top = s->base;
	s->size = STACK_INIT_SIZE;
	return OK;
}

Status DestroyStack_op(opStack *s){
	free(s->base);
	s->base = NULL;
	return OK;
}

op GetTop_op(opStack const *s){
	if (s->top == s->base) return ERROR;
	return *(s->top-1);
}

Status Push_op(opStack *s, op e){
	if (s->top - s->base >= s->size){
		s->base = (op *)realloc(s->base,(s->size+STACKINCREMENT)*sizeof(op));
		if (!s->base) exit(OVERFLOW);
		s->top = s->base + s->size;
		s->size += STACKINCREMENT;
	}
	*s->top++ = e;
	return OK;
}

Status Pop_op(opStack *s, op *e){
	if(s->top == s->base) return ERROR;
	*e = *--s->top;
	return OK;
}

Status InitStack_com(comStack *s){
	s->base = (complex *)malloc(STACK_INIT_SIZE * sizeof(complex));
	if (!s->base) exit(OVERFLOW);
	s->top = s->base;
	s->size = STACK_INIT_SIZE;
	return OK;
}

Status DestroyStack_com(comStack *s){
	free(s->base);
	s->base = NULL;
	return OK;
}

complex GetTop_com(comStack const *s){
	if (s->top == s->base) exit(ERROR);
	return *(s->top-1);
}

Status Push_com(comStack *s, complex *e){
	if (s->top - s->base >= s->size){
		s->base = (complex *)realloc(s->base,(s->size+STACKINCREMENT)*sizeof(complex));
		if (!s->base) exit(OVERFLOW);
		s->top = s->base + s->size;
		s->size += STACKINCREMENT;
	}
	*s->top++ = *e;
	return OK;
}

Status Pop_com(comStack *s, complex *e){
	if(s->top == s->base) return ERROR;
	*e = *--s->top;
	return OK;
}





