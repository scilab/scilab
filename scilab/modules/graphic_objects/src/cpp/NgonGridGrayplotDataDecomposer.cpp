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
#include "DecompositionUtils.hxx"
#include "NgonGridGrayplotDataDecomposer.hxx"

extern "C"
{
#include <stdlib.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

NgonGridGrayplotDataDecomposer* NgonGridGrayplotDataDecomposer::decomposer = NULL;

/*
 * To do: allowing either per-vertex or per-facet colors
 */
void NgonGridGrayplotDataDecomposer::fillColors(int id, float* buffer, int bufferLength, int elementsSize)
{
    int parentFigure = 0;
    int* pparentFigure = &parentFigure;
    int parent = 0;
    int* pparent = &parent;

    double* z = NULL;
    double* colormap = NULL;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;
    int dataMapping = 0;
    int* piDataMapping = &dataMapping;

    NgonGridGrayplotDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    parent = getParentObject(id);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (parent == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &pparentFigure);

    if (parentFigure == 0)
    {
        return;
    }

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    getGraphicObjectProperty(id, __GO_DATA_MAPPING__, jni_int, (void**) &piDataMapping);

    if (dataMapping == 0)
    {
        decomposer->fillNormalizedZGridColors(buffer, bufferLength, elementsSize, colormap, colormapSize, z, numX, numY);
    }
    else if (dataMapping == 1)
    {
        decomposer->fillDirectGridColors(buffer, bufferLength, elementsSize, colormap, colormapSize, z, numX, numY);
    }

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
}

int NgonGridGrayplotDataDecomposer::fillIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* x = NULL;
    double* y = NULL;
    double* z = NULL;
    double zShift = 0.0;
    double* pdZShift = &zShift;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int dataMapping = 0;
    int* piDataMapping = &dataMapping;

    int perNodeValues = 1;

    int numberIndices = 0;

    NgonGridGrayplotDataDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    /* 0 indices if 0 points */
    if (numX == 0 || numY == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MAPPING__, jni_int, (void**) &piDataMapping);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double, (void**) &pdZShift);

    /*
     * Data mapping values 0 and 1 respectively correspond to scaled and direct colors, for which per-node
     * and per-facet values are respectively used.
     */
    perNodeValues = !dataMapping;

    numberIndices = decomposer->fillTriangleIndices(buffer, bufferLength, logMask, x, y, &zShift, z, perNodeValues, numX, numY);

    return numberIndices;
}

int NgonGridGrayplotDataDecomposer::isFacetValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed, int currentEdgeValid, int* nextEdgeValid)
{
    *nextEdgeValid = isFacetEdgeValid(z, values, perNodeValues, numX, numY, i + 1, j, logUsed);

    if (!perNodeValues)
    {
        /*
         * Grid values are defined per facet.
         * In addition to edge validity, which in this case is determined only by z coordinates, the
         * facet's (i,j) value must also be read in order to determine its overall validity.
         */
        double zij = getValue(values, numX, numY, i, j);

        if (DecompositionUtils::isValid(zij) && currentEdgeValid && *nextEdgeValid)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        /*
         * Grid values are defined per node, and have therefore been already used
         * to determine edge validity
         */
        if (currentEdgeValid && *nextEdgeValid)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

/*
 * To be merged with its parent's isFacetEdgeValid function.
 */
int NgonGridGrayplotDataDecomposer::isFacetEdgeValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed)
{
    double zij = 0.;
    double zijp1 = 0.;

    int lowerZValid = 0;
    int upperZValid = 0;

    /* First, z-coordinate values are tested */
    zij = getZCoordinate(z, numX, numY, i, j, logUsed);
    zijp1 = getZCoordinate(z, numX, numY, i, j + 1, logUsed);

    lowerZValid = DecompositionUtils::isValid(zij);
    upperZValid = DecompositionUtils::isValid(zijp1);

    if (logUsed)
    {
        lowerZValid &= DecompositionUtils::isLogValid(zij);
        upperZValid &= DecompositionUtils::isLogValid(zijp1);
    }

    /*
     * If values are defined per node, edge validity must also
     * take into account grid values at the edge's nodes.
     */
    if (perNodeValues)
    {
        zij = getValue(values, numX, numY, i, j);
        zijp1 = getValue(values, numX, numY, i, j + 1);

        lowerZValid &= DecompositionUtils::isValid(zij);
        upperZValid &= DecompositionUtils::isValid(zijp1);
    }

    if (lowerZValid && upperZValid)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

