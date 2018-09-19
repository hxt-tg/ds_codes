/* expression.h */

#include "stack.h"
#include "complex.h"
#include "error.h"

#ifndef OP_LEN
#define OP_LEN 7
#define VALID_LEN 19
#define NUM_MAX 12
#define EXP_MAX 200
#endif

int Precede(op a, op b);
int InOP(op x);

/* String Process Part */
Status checkExp(char *src);
Status preProcess(char *src);


/* Calculate Part */
complex EvaluateExpression(char *src);
complex Operate(complex *a, op x, complex *b);




