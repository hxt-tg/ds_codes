/* stack.h */

#ifndef ERROR
#include "basic.h"
#endif
#define INFEASIBLE	-1
#define OVERFLOW	-2
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef char op;

typedef struct {
	op *base;
	op *top;
	int size;
} opStack;

typedef struct {
	complex *base;
	complex *top;
	int size;
} comStack;

// operator stack
Status InitStack_op(opStack *s);
Status DestroyStack_op(opStack *s);
op GetTop_op(opStack const *s);
Status Push_op(opStack *s, op e);
Status Pop_op(opStack *s, op *e);

// Complex stack
Status InitStack_com(comStack *s);
Status DestroyStack_com(comStack *s);
complex GetTop_com(comStack const *s);
Status Push_com(comStack *s, complex *e);
Status Pop_com(comStack *s, complex *e);


