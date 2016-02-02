/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
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

#include "ColorComputer.hxx"

extern "C"
{
#include <math.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

void ColorComputer::getColor(double s, double smin, double srange, double indexOffset, double* colormap, int colormapSize, float* returnedColor)
{
    double value = 0.;
    int index = 0;

    if (!DecompositionUtils::isANumber(s))
    {
        /* Black is output if s is a Nan */
        returnedColor[0] = MIN_COMPONENT_VALUE;
        returnedColor[1] = MIN_COMPONENT_VALUE;
        returnedColor[2] = MIN_COMPONENT_VALUE;
    }
    else
    {
        value = (s - smin) / (srange);
        index = (int) ((double)(colormapSize - 1) * value + indexOffset);

        /* Clamp */
        if (index < 0)
        {
            index = 0;
        }
        else if (index > colormapSize - 1)
        {
            index = colormapSize - 1;
        }

        returnedColor[0] = (float)colormap[index];
        returnedColor[1] = (float)colormap[colormapSize + index];
        returnedColor[2] = (float)colormap[2 * colormapSize + index];
    }
}

void ColorComputer::getColor(double s, double smin, double srange, double indexOffset, double* colormap, int minIndex, int maxIndex, int colormapSize, float* returnedColor)
{
    double value = 0.;
    int index = 0;

    if (!DecompositionUtils::isANumber(s))
    {
        /* Black is output if s is a Nan */
        returnedColor[0] = MIN_COMPONENT_VALUE;
        returnedColor[1] = MIN_COMPONENT_VALUE;
        returnedColor[2] = MIN_COMPONENT_VALUE;
    }
    else
    {
        value = (s - smin) / (srange);
        index = (int) ((double)(maxIndex - minIndex) * value + indexOffset + (double) minIndex);

        /* Clamp */
        if (index < minIndex)
        {
            index = minIndex;
        }
        else if (index > maxIndex)
        {
            index = maxIndex;
        }

        returnedColor[0] = (float)colormap[index];
        returnedColor[1] = (float)colormap[colormapSize + index];
        returnedColor[2] = (float)colormap[2 * colormapSize + index];
    }
}

double ColorComputer::getIndex(double s, double smin, double srange, double indexOffset, int minIndex, int maxIndex)
{
    double value = 0.;
    double index = 0.;

    if (!DecompositionUtils::isANumber(s))
    {
        /* Black is output if s is a Nan. */
        index = BLACK_INDEX;
    }
    else
    {
        value = (s - smin) / (srange);
        index = (double)(maxIndex - minIndex) * value + indexOffset + (double) minIndex;

        index = floor(index);

        /* Clamp */
        if (index < (double) minIndex)
        {
            index = (double) minIndex;
        }
        else if (index > (double) maxIndex)
        {
            index = (double) maxIndex;
        }
    }

    return index;
}

double ColorComputer::getDirectIndex(double s, int colormapSize)
{
    double index = 0.;

    if (s <= (double) WHITE_LOWER_INDEX)
    {
        /* Lowest index if the color is below the white range's lower index */
        index = 0.0;
    }
    else if (s <= (double) BLACK_LOWER_INDEX)
    {
        /* White is output */
        index = WHITE_INDEX;
    }
    else if ((((double) BLACK_LOWER_INDEX < s) && (s < (double) BLACK_UPPER_INDEX)) || !DecompositionUtils::isANumber(s))
    {
        /* Black is also output for Nan values */
        index = BLACK_INDEX;
    }
    else
    {
        index = floor(s);

        if (index > (double)(colormapSize - 1))
        {
            index = (double) (colormapSize - 1);
        }
    }

    return index;
}

double ColorComputer::getClampedDirectIndex(double s, int colormapSize)
{
    double index = s;

    /* Black is output if s is a Nan or if it corresponds to the black color */
    if (!DecompositionUtils::isANumber(s) || ((BLACK_INDEX <= s) && (s < 0.0)))
    {
        index = BLACK_INDEX;
    }
    else
    {
        index = floor(index);

        /* Clamp */
        if (s < 0.0)
        {
            index = 0.0;
        }
        else if (s > (double)(colormapSize - 1))
        {
            index = (double) (colormapSize - 1);
        }
    }

    return index;
}

void ColorComputer::getClampedDirectColor(double s, double* colormap, int colormapSize, float* returnedColor)
{
    int index = 0;

    if (!DecompositionUtils::isANumber(s))
    {
        /* Black is output if s is a Nan */
        returnedColor[0] = MIN_COMPONENT_VALUE;
        returnedColor[1] = MIN_COMPONENT_VALUE;
        returnedColor[2] = MIN_COMPONENT_VALUE;
    }
    else
    {
        /* Clamp */
        if (s < 0.0)
        {
            s = 0.0;
        }
        else if (s > (double)(colormapSize - 1))
        {
            s = (double) (colormapSize - 1);
        }

        index = (int) s;

        returnedColor[0] = (float)colormap[index];
        returnedColor[1] = (float)colormap[colormapSize + index];
        returnedColor[2] = (float)colormap[2 * colormapSize + index];
    }
}
