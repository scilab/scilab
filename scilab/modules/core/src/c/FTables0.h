#ifndef __FTABLES0__
#define __FTABLES0__

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

/***********************************
 * Search Table for fbutn
 ***********************************/


#define ARGS_fbutn char *,integer*,integer*
typedef integer * (*fbutnf)(ARGS_fbutn);



/***********************************
 * Search Table for interf
 ***********************************/

#define ARGS_interf void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *,void *
typedef integer * (*interff)(ARGS_interf);


#define ARGS_interf1 char *,long int
typedef integer * (*interff1)(ARGS_interf1);

#endif /*__FTABLES0__*/
