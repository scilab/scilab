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
#include <float.h>
#ifdef _MSC_VER
#define isnan _isnan
    // isinf(x)
    // | +%inf -> 1
    // | -%inf -> -1
    // | _ -> 0
#define isinf(x) (_fpclass(x)==_FPCLASS_PINF?1:(_fpclass(x)==_FPCLASS_NINF?-1:0))
#endif
}

int DecompositionUtils::isANumber(double x)
{
    if (isnan(x))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int DecompositionUtils::isFinite(double x)
{
    if (isinf(x))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int DecompositionUtils::isValid(double x)
{
    if (isnan(x) || isinf(x))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int DecompositionUtils::isValid(double x, double y, double z)
{
    if (isnan(x) || isnan(y) || isnan(z) || isinf(x) || isinf(y) || isinf(z))
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

int DecompositionUtils::isLogValid(double x)
{
    if (x > 0.0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
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

double DecompositionUtils::getMaxDoubleValue(void)
{
    return DBL_MAX;
}

double DecompositionUtils::getMinDoubleValue(void)
{
    return DBL_MIN;
}

double DecompositionUtils::getAbsoluteValue(double value)
{
    return fabs(value);
}

double DecompositionUtils::getSquareRoot(double value)
{
    return sqrt(value);
}

/*
 * Decomposes a rectangle into two adjacent triangles.
 * The rectangle's vertices are supposed to be specified in
 * counter-clockwise order, with 0 corresponding to the former's lower-left vertex.
 * The two output triangles' vertex indices are also specified in
 * counter-clockwise order.
 */
void DecompositionUtils::getDecomposedRectangleTriangleIndices(int* indices)
{
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
}

