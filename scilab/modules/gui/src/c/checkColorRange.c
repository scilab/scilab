/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 * Return the style name corresponding to the int value from UicontrolStyle enum
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "checkColorRange.h"

/*--------------------------------------------------------------------------*/
static int checkColorComponent(double c)
{
    return (c >= 0) && (c <= 1);
}
/*--------------------------------------------------------------------------*/
int checkColorRange(double r, double g, double b)
{
    if (r == -1 && g == -1 && b == -1) //default laf
    {
        return 1;
    }

    if (r == -2 && g == -2 && b == -2) //transparence
    {
        return 1;
    }

    return checkColorComponent(r) && checkColorComponent(g) && checkColorComponent(b);
}
/*--------------------------------------------------------------------------*/
