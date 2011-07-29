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
#include "Fac3DColorComputer.hxx"

extern "C"
{
#include <stdlib.h>
}

Fac3DColorComputer::Fac3DColorComputer(void)
{
    colors = NULL;
    numColors = 0;
    colorFlag = 0;
    dataMapping = 1;
    numGons = 0;
    numVerticesPerGon = 0;
    numColorsPerFacet = 0;
    perVertex = 0;
    minColorValue = 0.0;
    maxColorValue = 0.0;
    colorRange = 0.0;
    usedMinColorValue = 0.0;
    colorRangeValid = 0.0;
}

Fac3DColorComputer::Fac3DColorComputer(double* colors, int numColors, int colorFlag, int dataMapping, int numGons, int numVerticesPerGon)
{
    this->colors = colors;
    this->numColors = numColors;
    this->colorFlag = colorFlag;
    this->dataMapping = dataMapping;
    this->numGons = numGons;
    this->numVerticesPerGon = numVerticesPerGon;

    if (numColors == numGons*numVerticesPerGon)
    {
        perVertex = 1;
        numColorsPerFacet = numVerticesPerGon;
    }
    else
    {
        perVertex = 0;
        numColorsPerFacet = 1;
    }

    /* Scaled */
    if (dataMapping == 0)
    {
        colorRangeValid = 1;

        computeMinMaxValues();

        colorRange = maxColorValue - minColorValue;
        usedMinColorValue = minColorValue;

        /* To be verified */
        if (colorRange < DecompositionUtils::getMinDoubleValue())
        {
            colorRangeValid = 0;

            /* Set the actually used color range to [0.0, 1.0] */
            usedMinColorValue = 0.0;
            colorRange = 1.0;
        }

    }

}

Fac3DColorComputer::~Fac3DColorComputer(void)
{

}

/* To do:
 * -rename getFacetColor to getFacetValue.
 * -streamline.
 */
double Fac3DColorComputer::getFacetColor(int facetIndex, int vertexIndex)
{
    double color = 0.0;

    /*
     * Special case for scaled colors: return the color at half the actually used color range.
     * To be fully implemented.
     */
    if (dataMapping == 0 && colorRangeValid == 0)
    {
        return 0.5 * (colorRange);
    }

    if (perVertex == 1)
    {
        if (colorFlag == 2)
        {
            color = computeAverageValue(facetIndex);
        }
        else if (colorFlag == 3)
        {
            color = colors[facetIndex*numVerticesPerGon + vertexIndex];
        }
        else if (colorFlag == 4)
        {
            color = colors[facetIndex*numVerticesPerGon];
        }
    }
    else if (perVertex == 0)
    {
        color = colors[facetIndex];
    }

    return color;
}

/*
 * To do:
 * -complete and implement the scaled colors case.
 * -streamline.
 */
int Fac3DColorComputer::isFacetColorValid(int facetIndex)
{
    double color;
    int valid;

    /* Special case, no colors therefore facets are always valid */
    if (colorFlag == 0 || colorFlag == 1 || numColors == 0)
    {
        return 1;
    }

    /* Special case for colorFlag == 3 && perVertex == 1 */
    if (colorFlag == 3 && perVertex == 1)
    {
        /* To do: implement according to data mapping */
        valid = 0;

        for (int i = 0; i < numVerticesPerGon; i++)
        {
            color = getFacetColor(facetIndex, i);

            /*
             * The facet is valid if at least one of its vertex colors is different from 0
             * and is a valid number (direct mapping).
             */
            if (dataMapping == 1)
            {
                if (color != 0.0 && DecompositionUtils::isANumber(color))
                {
                    valid = 1;
                    break;
                }
            }
            else if (dataMapping == 0)
            {
                if (DecompositionUtils::isANumber(color))
                {
                    valid = 1;
                    break;
                }
            }
        }

    }
    else
    {
        /* Vertex index set to 0 */
        color = getFacetColor(facetIndex, 0);

        if (color == 0.0 || DecompositionUtils::isANumber(color) == 0)
        {
            valid = 0;
        }
        else
        {
            valid = 1;
        }
    }

    return valid;
}

double Fac3DColorComputer::computeAverageValue(int facetIndex)
{
    double averageValue;
    int i;

    averageValue = 0.0;

    for (i = 0; i < numVerticesPerGon; i++)
    {
        averageValue += colors[numVerticesPerGon*facetIndex + i];
    }

    averageValue /= (double) numVerticesPerGon;

    return averageValue;
}

void Fac3DColorComputer::computeMinMaxValues(void)
{
    double maxDouble;
    double tmpValueMin;
    double tmpValueMax;
    double value;

    int i;

    maxDouble = DecompositionUtils::getMaxDoubleValue();

    tmpValueMin = maxDouble;
    tmpValueMax = -maxDouble;

    for (i = 0; i < numGons; i++)
    {
        computeFacetMinMax(i, &tmpValueMin, &tmpValueMax);
    }

    minColorValue = tmpValueMin;
    maxColorValue = tmpValueMax;
}

void Fac3DColorComputer::computeFacetMinMax(int facetIndex, double* currentMin, double* currentMax)
{
    double value;

    for (int i = 0; i < numColorsPerFacet; i++)
    {
        value = getFacetColor(facetIndex, i);

        if (DecompositionUtils::isValid(value))
        {
            if (value < *currentMin)
            {
                *currentMin = value;
            }

            if (value > *currentMax)
            {
                *currentMax = value;
            }
        }
    }

}

void Fac3DColorComputer::getColorRangeValue(double* minColorValue, double* colorRange)
{
    *minColorValue = usedMinColorValue;
    *colorRange = this->colorRange;
}

