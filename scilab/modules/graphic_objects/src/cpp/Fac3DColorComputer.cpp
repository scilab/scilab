/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
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
    /* 1: direct mapping */
    dataMapping = 1;
    numGons = 0;
    numVerticesPerGon = 0;
    numColorsPerFacet = 0;
    perVertex = 0;
    minColorValue = 0.0;
    maxColorValue = 0.0;
    colorRange = 0.0;
    usedMinColorValue = 0.0;
    colorRangeValid = 0;
}

Fac3DColorComputer::Fac3DColorComputer(double* colors, int numColors, int colorFlag, int dataMapping, int numGons, int numVerticesPerGon)
{
    this->colors = colors;
    this->numColors = numColors;
    this->colorFlag = colorFlag;
    this->dataMapping = dataMapping;
    this->numGons = numGons;
    this->numVerticesPerGon = numVerticesPerGon;

    if (numColors == numGons * numVerticesPerGon)
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
        if (numColors == 0)
        {
            colorRangeValid = 0;

            /* Set the actually used color range to [0.0, 1.0] */
            usedMinColorValue = 0.0;
            colorRange = 1.0;
        }
        else
        {
            colorRangeValid = 1;
            computeMinMaxValues();

            colorRange = maxColorValue - minColorValue;
            usedMinColorValue = minColorValue;

            /*
             * The color range is invalid when its minimum and maximum values are equal.
             * This does not matter when all the color values are either infinite or Nans,
             * as facets will be considered as invalid anyway.
             */
            if (colorRange < DecompositionUtils::getMinDoubleValue())
            {
                colorRangeValid = 0;

                /* Set the actually used color range to [0.0, 1.0] */
                usedMinColorValue = 0.0;
                colorRange = 1.0;
            }
        }
    }

}

Fac3DColorComputer::~Fac3DColorComputer(void)
{

}

int Fac3DColorComputer::isInterpolatedShadingUsed(void)
{
    int interpolatedShading = 0;

    /*
     * Interpolated shading is used only when colors are defined per vertex
     * and color flag is equal to 3. In any other case, flat shading is used.
     */
    if (perVertex == 1 && colorFlag == 3)
    {
        interpolatedShading = 1;
    }

    return interpolatedShading;
}

double Fac3DColorComputer::getOutputFacetColor(int facetIndex, int vertexIndex)
{
    /*
     * Special case for scaled colors: return the color at half
     * the used color range if it is not value, whatever perVertex's value.
     */
    if (dataMapping == 0 && colorRangeValid == 0)
    {
        return 0.5 * (colorRange);
    }
    else
    {
        return getFacetColor(facetIndex, vertexIndex);
    }

}

/* To do:
 * -rename getFacetColor to getFacetValue. or getVertexColorIndex
 * -streamline.
 */
double Fac3DColorComputer::getFacetColor(int facetIndex, int vertexIndex)
{
    double color = 0.0;

    if (perVertex == 1)
    {
        if (colorFlag == 2)
        {
            color = computeAverageValue(facetIndex);
        }
        else if (colorFlag == 3)
        {
            color = colors[facetIndex * numVerticesPerGon + vertexIndex];
        }
        else if (colorFlag == 4)
        {
            color = colors[facetIndex * numVerticesPerGon];
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
 * -streamline.
 */
int Fac3DColorComputer::isFacetColorValid(int facetIndex)
{
    double color = 0.;
    int valid = 0;

    /* Special case, no colors therefore facets are always valid */
    if (colorFlag == 0 || colorFlag == 1 || numColors == 0)
    {
        return 1;
    }

    /* Special case for colorFlag == 3 and perVertex color values */
    if (colorFlag == 3 && perVertex == 1)
    {
        valid = 0;

        for (int i = 0; i < numVerticesPerGon; i++)
        {
            color = getFacetColor(facetIndex, i);

            if (dataMapping == 1)
            {
                /*
                 * The facet is valid if at least one of its vertex colors is not in the interval
                 * [0,1) and is a valid number.
                 */
                if ((int) color != 0 && DecompositionUtils::isANumber(color))
                {
                    valid = 1;
                    break;
                }
            }
            else if (dataMapping == 0)
            {
                /* If at least one color value is infinite, the facet is considered as invalid. */
                if (!DecompositionUtils::isFinite(color))
                {
                    valid = 0;
                    break;
                }

                /* If at least one value is a number, the facet is valid, provided it has no infinite values. */
                if (DecompositionUtils::isANumber(color) && valid == 0)
                {
                    valid = 1;
                }

            }
        }

    }
    else
    {
        /* Vertex index set to 0 as all the vertices of a given facet have the same color value */
        color = getFacetColor(facetIndex, 0);

        if (dataMapping == 1)
        {
            /* The facet is valid if its color value is not in the interval [0,1) and is a valid number. */
            if ((int) color == 0 || DecompositionUtils::isANumber(color) == 0)
            {
                valid = 0;
            }
            else
            {
                valid = 1;
            }
        }
        else if (dataMapping == 0)
        {
            /* The facet is valid if its color value is valid. */
            if (DecompositionUtils::isValid(color))
            {
                valid = 1;
            }
            else
            {
                valid = 0;
            }

        }
    }

    return valid;
}

double Fac3DColorComputer::computeAverageValue(int facetIndex)
{
    double averageValue = 0.;
    int i = 0;

    averageValue = 0.0;

    for (i = 0; i < numVerticesPerGon; i++)
    {
        averageValue += colors[numVerticesPerGon * facetIndex + i];
    }

    averageValue /= (double) numVerticesPerGon;

    return averageValue;
}

void Fac3DColorComputer::computeMinMaxValues(void)
{
    double maxDouble = 0.;
    double tmpValueMin = 0.;
    double tmpValueMax = 0.;

    int i = 0;

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
    double value = 0.;

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

