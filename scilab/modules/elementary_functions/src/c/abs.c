/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Antoine ELIAS
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
#include <math.h>
#include "abs.h"
#include "elem_common.h"
/*abs*/
double dabss(double _dblVal)
{
    return fabs(_dblVal);
}

/*module of complex*/
double dabsz(double _dblRealVal, double _dblImgVal)
{
    double dblAbsReal	= dabss(_dblRealVal);
    double dblAbsImg	= dabss(_dblImgVal);
    double dblMax		= Max(dblAbsReal, dblAbsImg);
    double dblMin		= Min(dblAbsReal, dblAbsImg);
    if (dblMin == 0)
    {
        return dblMax;
    }
    else
    {
        return dblMax * (sqrt(1 + pow(dblMin / dblMax, 2)));
    }
    /*
    W = MAX( XABS, YABS )
    Z = MIN( XABS, YABS )
    */
}
