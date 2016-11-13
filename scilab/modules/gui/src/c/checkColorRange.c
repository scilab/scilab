/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 * Return the style name corresponding to the int value from UicontrolStyle enum
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
