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

#include "ColorComputer.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

void ColorComputer::getColor(double s, double smin, double srange, double* colorMap, int colormapSize, float* returnedColor)
{
    double value;
    int index;

    value = (s - smin) / (srange);
    index = (int) ((double)(colormapSize-1)*value);

    returnedColor[0] = colorMap[index];
    returnedColor[1] = colorMap[colormapSize+index];
    returnedColor[2] = colorMap[2*colormapSize+index];
}

