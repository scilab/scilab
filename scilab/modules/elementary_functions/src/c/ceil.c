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
#include "basic_functions.h"


/*up round*/
double dceils(double _dblVal)
{
    /*
    3.14 -> 3
    4,00 -> 4

    -3.14 -> -3
    -4.00 -> -4
    */
    return ceil(_dblVal);
}

/*up round with precision*/
double dceilsEx(double _dblVal, int _iPrecision)
{
    double iPow = pow(10, _iPrecision);

    double dblTemp = _dblVal * iPow;
    dblTemp = dceils(dblTemp);

    return (double)dblTemp / iPow;

}
