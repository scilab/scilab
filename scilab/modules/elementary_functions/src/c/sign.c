/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
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
#include "elem_common.h"


/*sign*/
double dsigns(double _dblRef, double _dblVal)
{
    if ( _dblVal >= 0)
    {
        return _dblRef;
    }
    else
    {
        return -_dblRef;
    }
}

double dsignsEx(double _dblVal)
{
    if (_dblVal == 0)
    {
        return 0;
    }
    if (_dblVal > 0)
    {
        return 1;
    }
    if (_dblVal < 0)
    {
        return -1;
    }
    return _dblVal;

}
