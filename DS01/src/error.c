/* error.c */

#include "error.h"
#include <stdio.h>

/* Error precessing module(Print error info) */
Status errorProcess(int pos, ERR_MSG err){
	if (err == RIGHT)
		return OK;
	else{
		int i;
		if (pos >= 0){	/* Locate error position */
			for (i = 0; i < pos+11;i++)
				printf(" ");
			printf("^\n");
		}
		printf("!   [Error] ");
		switch (err){
			case INVALID_CHAR:
				printf("Invalid character.\n");
				break;
			case INVALID_DOT:
				printf("Invalid dot.\n");
				break;
			case LOST_CLOSE_BRACKET:
				printf("Expected close bracket.\n");
				break;
			case LOST_OPEN_BRACKET:
				printf("Unpaired bracket.\n");
				break;
			case LOST_OPND:
				printf("Expected operand.\n");
				break;
			case LOST_OPND_MINUS:
				printf("Expected operand. (You may need a pair of brackets)\n");
				break;
			case LOST_OPTR:
				printf("Expected operator.\n");
				break;
			case NUM_OVERFLOW:
				printf("This number is too big.\n");
				break;
			case UNEXPECTED_DOT:
				printf("Unexpected dot. (You may need type a 0)\n");
				break;
			case DIV_ZERO:
				printf("Dividend is 0.\n");
				break;
			case INVALID_REUSE:
				printf("Invalid format of reusing previous result.\n");
				break;
		}
		return ERROR;
	}
		
}
