/* complex.h */

#ifndef ERROR
#include "basic.h"
#endif

int DIV_ERR_MSG;

/* Input and output string-processing mode */
double readOneNumber(char *src);
char *floatSimplify(char *str);
int printCom(complex const *src);
int sprintCom(char *dest, complex const *src);

/* Evaluate module */
complex *addCom(complex const *a, complex const *b);
complex *multiplyCom(complex const *a, complex const *b);
complex *divideCom(complex const *a, complex const *b);





