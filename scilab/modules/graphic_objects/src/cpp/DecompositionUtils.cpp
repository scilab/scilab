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

