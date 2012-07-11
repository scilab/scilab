/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "setformat.h"
#include "stack-c.h"

/*--------------------------------------------------------------------------*/
void setformat(const char * format, int width)
{
    C2F(iop).lct[5] = *format == 'e' ? 0 : 1;
    C2F(iop).lct[6] = width;
}
/*--------------------------------------------------------------------------*/
char * getformat()
{
    return C2F(iop).lct[5] ? "v" : "e";
}
/*--------------------------------------------------------------------------*/
int getformatwidth()
{
    return C2F(iop).lct[6];
}
