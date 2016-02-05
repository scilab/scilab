/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

