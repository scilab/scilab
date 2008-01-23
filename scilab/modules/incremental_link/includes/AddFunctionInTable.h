/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/*--------------------------------------------------------------------------*/
#ifndef __ADDFUNCTIONINTABLE_H__
#define __ADDFUNCTIONINTABLE_H__

#include "machine.h"

typedef void (*voidf)();

/* structure entry in functions table */

typedef struct {
	char *name; /* function name */
	voidf f; /* pointer on function */
} FTAB;


/**
* Add a C or fortran function in functions hashtable
*
* @param name  function name
* @param[OUT] rep  result 1 (OK) or 0 (Problem)
* @param table functions hashtable
* @return a pointer on function in functions hashtable
*/
voidf AddFunctionInTable (char *name, int *rep, FTAB *table);  

#endif /* __ADDFUNCTIONINTABLE_H__ */
/*--------------------------------------------------------------------------*/

