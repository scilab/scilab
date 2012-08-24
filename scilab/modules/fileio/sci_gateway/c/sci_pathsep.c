/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_pathsep(char *fname, unsigned long fname_len)
{
    static int n1, m1;
    char *separator = NULL;

    CheckRhs(0, 0);
    CheckLhs(1, 1);

    separator = (char*)MALLOC(sizeof(char) * (strlen(PATH_SEPARATOR) + 1));
    if (separator) strcpy(separator, PATH_SEPARATOR);

    n1 = 1;
    m1 = (int)strlen(separator);
    CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &m1, &n1, &separator);
    LhsVar(1) = Rhs + 1;

    if (separator)
    {
        FREE(separator);
        separator = NULL;
    }
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
