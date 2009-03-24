
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __ADDFUNCTIONINTABLE_H__
#define __ADDFUNCTIONINTABLE_H__


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
* @param[OUT] rep  result 0 (OK) or 1 (Problem)
* @param table functions hashtable
* @return a pointer on function in functions table
*/
voidf AddFunctionInTable (char *name, int *rep, FTAB *table);  

#endif /* __ADDFUNCTIONINTABLE_H__ */
/*--------------------------------------------------------------------------*/

