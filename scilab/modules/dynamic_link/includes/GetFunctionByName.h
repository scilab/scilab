/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GETFUNCTIONBYNAME_H__
#define __GETFUNCTIONBYNAME_H__

#include "dynlib_dynamic_link.h"

typedef void (*voidf)();

/* structure entry in functions table */

typedef struct
{
    char *name; /* function name */
    voidf f; /* pointer on function */
} FTAB;


/**
* Gets a pointer to a C or Fortran function by name from the functions table
*
* @param name  function name
* @param[OUT] rep  result 1 (OK) or 0 (Problem)
* @param table functions hashtable
* @return a pointer on function in functions hashtable
*/
DYNAMIC_LINK_IMPEXP voidf GetFunctionByName (char *name, int *rep, FTAB *table);

#endif /* __GETFUNCTIONBYNAME_H__ */
/*--------------------------------------------------------------------------*/

