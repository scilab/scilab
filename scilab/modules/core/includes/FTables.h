/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#ifndef __FTABLES__
#define __FTABLES__
#include "machine.h"

#define OK 1
#define FAIL 0

typedef void (*voidf)();

typedef struct { double r, i; } doublecmplx;

typedef struct {
	char *name;
	voidf f;
} FTAB;

#define MAXNAME 32
static char buf[MAXNAME];

voidf SetFunction(char *name, int *rep, FTAB *table);


#endif /*__FTABLES__*/
