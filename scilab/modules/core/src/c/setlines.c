/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "setlines.h"
#include "stack-c.h"

/*--------------------------------------------------------------------------*/
void setlines(int lines, int columns)
{
    C2F(iop).lct[4] = Max(25, columns);
    C2F(iop).lct[1] = Max(0, lines);
}
/*--------------------------------------------------------------------------*/
