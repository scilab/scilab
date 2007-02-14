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
 * Search Table for fschur
 ***********************************/

#define ARGS_fschur integer*,double *,double*,double*,double*
typedef integer * (*fschurf)(ARGS_fschur);

/***********************************
 * Search Table for schsel
 ***********************************/

#define ARGS_schsel double *,double*
typedef integer * (*schself)(ARGS_schsel);

/***********************************
 * Search Table for zchsel
 ***********************************/

#define ARGS_zchsel doublecmplx *
typedef integer * (*zchself)(ARGS_zchsel);

/***********************************
 * Search Table for gshsel
 ***********************************/

#define ARGS_gshsel double *,double*,double*
typedef integer * (*gshself)(ARGS_gshsel);

/***********************************
 * Search Table for gzhsel
 ***********************************/

#define ARGS_gzhsel doublecmplx *,doublecmplx *
typedef integer * (*gzhself)(ARGS_gzhsel);




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
