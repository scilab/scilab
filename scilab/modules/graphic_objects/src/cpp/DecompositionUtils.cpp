/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DecompositionUtils.hxx"

extern "C"
{
#include <math.h>
#ifdef _MSC_VER
#include <float.h>
#define isnan _isnan
#endif
}

int DecompositionUtils::isValid(double x, double y, double z)
{
    if (isnan(x) || isnan(y) || isnan(z))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

double DecompositionUtils::getLog10Value(double value)
{
    return log10(value);
}

int DecompositionUtils::isLogValid(double x, double y, double z, int logMask)
{
    int valid = 1;

    if (logMask & 0x1)
    {
        valid &= (x > 0.0);
    }

    if (logMask & 0x2)
    {
        valid &= (y > 0.0);
    }

    if (logMask & 0x4)
    {
        valid &= (z > 0.0);
    }

    return valid;
}

