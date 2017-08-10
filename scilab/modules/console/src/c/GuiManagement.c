/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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
#include "GuiManagement.h"
#include "scilines.h"
#include "configvariable_interface.h"
/*--------------------------------------------------------------------------*/
void setScilabLines(int nbRows, int nbCols)
{
    if (getConsoleLines() == 0)
    {
        /* The user does not want to be asked for more display */
        /* Modify only the number of columns used to format displayed data */
        scilines(getConsoleLines(), nbCols);
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
