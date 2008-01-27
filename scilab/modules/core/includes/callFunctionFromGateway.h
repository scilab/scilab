/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __CALLFUNCTIONFROMGATEWAY_H__
#define __CALLFUNCTIONFROMGATEWAY_H__

#include "machine.h"

typedef int (*function_Interf) __PARAMS((char *fname,unsigned long l));

/** 
 * See SCI/modules/<module_name>/sci_gateway/c/gw_<module_name>.c for example 
*/
typedef struct functions_table_struct {
	function_Interf f; /** the function itself **/
	char *name;      /** its name in Scilab **/
} gw_generic_table;


/**
* call a function associated to a number "Fin-1"
* in Tab
* @param Tab gw_generic_table
*/
void callFunctionFromGateway(gw_generic_table *Tab);

#endif /* __CALLFUNCTIONFROMGATEWAY_H__ */
/*--------------------------------------------------------------------------*/
