/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "GuiManagement.h"
#include "scilines.h"
/*--------------------------------------------------------------------------*/
void setScilabLines(int nbRows, int nbCols)
{
    if (getLinesSize() == 0)
    {
        /* The user does not want to be asked for more display */
        /* Modify only the number of columns used to format displayed data */
        scilines(getLinesSize(), nbCols);
    }
    else
    {
        /* Modify number of lines and columns used to format displayed data */
        scilines(nbRows, nbCols);
    }
}
/*--------------------------------------------------------------------------*/
void forceScilabLines(int nbRows, int nbCols)
{
    /* Modify number of lines and columns used to format displayed data */
    scilines(nbRows, nbCols);
}
/*--------------------------------------------------------------------------*/
