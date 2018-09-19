/* error.h */

#ifndef ERROR
#include "basic.h"
#endif

typedef enum{
	RIGHT,				/* No error */
	INVALID_CHAR,		/* Find char which is not in VALID_SET */
	INVALID_DOT,		/* ..	.<num>.	*/
	LOST_CLOSE_BRACKET,	/* (() */
	LOST_OPEN_BRACKET,	/* ()) */
	LOST_OPND,			/* [+,-,*,/][),+,*,/] (+   (*   (/ */
	LOST_OPND_MINUS,	/* [+,-,*,/]- */
	NUM_OVERFLOW,		/* digits to long */
	LOST_OPTR,			/* ii	i<num>i */
	INVALID_REUSE,		/* .p   pp   pi   p<num>*/
	UNEXPECTED_DOT,		/* .i	.<op> */
	DIV_ZERO
} ERR_MSG;

Status errorProcess(int pos, ERR_MSG err);

