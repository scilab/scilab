/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include <string.h>
#include "scilabWrite.hxx"

extern "C"
{
#include "basout.h"
int C2F(basout)(int *io, int *lunit, char *string, long int nbcharacters)
{
    char* strMsg = new char[nbcharacters+1];
    *io = 0;
    memcpy(strMsg, string, nbcharacters);
    strMsg[nbcharacters] = '\0';
    scilabForcedWrite(strMsg);
    scilabForcedWrite("\n");
    delete strMsg;
    return 0;
}
}

