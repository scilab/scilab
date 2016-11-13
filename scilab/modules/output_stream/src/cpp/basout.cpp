/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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

#include <string.h>
#include "scilabWrite.hxx"

extern "C"
{
#include "basout.h"
}

int C2F(basout)(int *io, int *lunit, char *string, long int nbcharacters)
{
    char* strMsg = new char[nbcharacters + 1];
    *io = 0;
    memcpy(strMsg, string, nbcharacters);
    strMsg[nbcharacters] = '\0';
    scilabForcedWrite(strMsg);
    scilabForcedWrite("\n");
    delete[] strMsg;
    return 0;
}

