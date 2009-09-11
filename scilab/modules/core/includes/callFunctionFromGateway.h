/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CALLFUNCTIONFROMGATEWAY_H__
#define __CALLFUNCTIONFROMGATEWAY_H__

#define SIZE_CURRENT_GENERIC_TABLE(tab) sizeof(tab) / sizeof(gw_generic_table)

typedef int (*function_Interf)(char *fname,unsigned long l);

/** 
 * See SCI/modules/<module_name>/sci_gateway/c/gw_<module_name>.c for example 
*/

typedef struct functions_table_struct {
	function_Interf f; /** the function itself **/
	char const * const name;      /** its name in Scilab **/
	/* char const * const , to remove some warnings -Wall (linux) */
} gw_generic_table;

/**
* call a function associated to a number "Fin-1"
* in Tab
* @param Tab gw_generic_table
* @param sizeTab size of Tab
*/
void callFunctionFromGateway(gw_generic_table *Tab,int sizeTab);

#endif /* __CALLFUNCTIONFROMGATEWAY_H__ */
/*--------------------------------------------------------------------------*/
