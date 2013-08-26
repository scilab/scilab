/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GETENTRIESHASHTABLE_H__
#define __GETENTRIESHASHTABLE_H__

#include <string.h>
#include "hashtable.h"

/* get an array of strings from hastable tableIN */
char **getTAGSinhashtable(struct hashtable *tableIN, int *numbersEntries);
char **getSTRINGSinhashtable(struct hashtable *tableIN, int *numbersEntries);
char **getPATHSinhashtable(struct hashtable *tableIN, int *numbersEntries);

#endif
/*--------------------------------------------------------------------------*/
