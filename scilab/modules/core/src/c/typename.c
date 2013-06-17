/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "typename.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "cvstr.h"
#include "BOOL.h"

static BOOL isTypeNameAlreadyExist(char *name);
/*--------------------------------------------------------------------------*/
char **getAllTypesName(int *sizeArray)
{
    char **TypeNames = NULL;
    return TypeNames;
}
/*--------------------------------------------------------------------------*/
int *getAllTypesNumber(int *sizeArray)
{
    int *TypeNumbers = NULL;
    return TypeNumbers;
}
/*--------------------------------------------------------------------------*/
int getNumberOfTypes(void)
{
    int NumberOfTypes = 0;
    return NumberOfTypes;
}
/*--------------------------------------------------------------------------*/
int addNamedType(char *name, int val)
{
    int ierr = 0;
    return ierr;
}
/*--------------------------------------------------------------------------*/
BOOL isTypeNameAlreadyExist(char *name)
{
    int sizeArray = 0;
    return FALSE;
}
/*--------------------------------------------------------------------------*/
