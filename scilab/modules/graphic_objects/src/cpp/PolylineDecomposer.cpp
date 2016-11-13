/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Pierre Lando
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

#include <iostream>

#include "DecompositionUtils.hxx"
#include "PolylineDecomposer.hxx"
#include "Triangulator.hxx"
#include "ColorComputer.hxx"

extern "C"
{
#include <math.h>
#include <string.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int PolylineDecomposer::getDataSize(int id)
{
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int closed = 0;
    int* piClosed = &closed;

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    /* If 0 points, 0 elements */
    if (nPoints == 0)
    {
        return 0;
    }

    switch (polylineStyle)
    {
        case 1 :
            /* Segments */
            return nPoints;
        case 2 :
            /* Staircase */
            if (closed)
            {
                return 2 * nPoints;
            }
            else
            {
                return (2 * nPoints) - 1;
            }
        case 3 :
            /* Vertical segments plus segments */
            return 2 * nPoints;
        case 4 :
            /* Segments with arrow heads */
        {
            int nArrowVertices;
            /* The numbers of arrow head vertices and indices are exactly the same */
            nArrowVertices = PolylineDecomposer::getArrowTriangleIndicesSize(nPoints, closed);

            return nPoints + nArrowVertices;
        }
        case 5 :
            /* Filled patch  */
            return nPoints;
        case 6 :
            /* Vertical bars plus segments */
            return 5 * nPoints;
        case 7 :
            /* Horizontal bars plus segments */
            return 5 * nPoints;
        default :
            /* To be done: remaining styles */
            return 0;
    }
}

void PolylineDecomposer::fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* t = NULL;
    double* xshift = NULL;
    double* yshift = NULL;
    double* zshift = NULL;

    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;

    int nPoints = 0;
    int *piNPoints = &nPoints;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &t);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void**) &xshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, (void**) &yshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double_vector, (void**) &zshift);

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);

    switch (polylineStyle)
    {
        case 1 :
            fillSegmentsDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
            break;
        case 2 :
            fillStairDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
            break;
        case 3 :
            fillVerticalLinesDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
            break;
        case 4 :
            fillSegmentsDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
            break;
        case 5 :
            fillSegmentsDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
            break;
        case 6 :
            fillVerticalBarsDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
            break;
        case 7 :
            fillHorizontalBarsDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
            break;
    }
}

void PolylineDecomposer::fillSegmentsDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{

    int componentIndices[3];

    // TODO Optimize ? (test if s = 1 and t = 0, coordinateMask = 0 ...)
    for (int i = 0; i < nPoints; i++)
    {
        /* Offset of a polyline vertex */
        int v0 = elementsSize * i;

        componentIndices[0] = i;
        componentIndices[1] = i;
        componentIndices[2] = i;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize,
                                  xshift, yshift, zshift, coordinateMask, scale, translation, logMask);
    }

}

void PolylineDecomposer::getAndWriteVertexToBuffer(float* buffer, int offset, double* coordinates, int* vertexIndices, int nPoints, int elementsSize,
        double* xshift, double* yshift, double* zshift, int coordinateMask, double* scale, double* translation, int logMask)
{
    double coordinate = 0.;

    if (coordinateMask & 0x01)
    {
        coordinate = coordinates[vertexIndices[0]];

        if (xshift != NULL)
        {
            coordinate += xshift[vertexIndices[0]];
        }

        if (logMask & 0x01)
        {
            coordinate = DecompositionUtils::getLog10Value(coordinate);
        }

        buffer[offset + 0] = (float)(coordinate * scale[0] + translation[0]);
    }

    if (coordinateMask & 0x02)
    {
        coordinate = coordinates[vertexIndices[1] + nPoints];

        if (yshift != NULL)
        {
            coordinate += yshift[vertexIndices[1]];
        }

        if (logMask & 0x02)
        {
            coordinate = DecompositionUtils::getLog10Value(coordinate);
        }

        buffer[offset + 1] = (float)(coordinate * scale[1] + translation[1]);
    }

    if (coordinateMask & 0x04)
    {
        coordinate = coordinates[vertexIndices[2] + 2 * nPoints];

        if (zshift != NULL)
        {
            coordinate += zshift[vertexIndices[2]];
        }

        if (logMask & 0x04)
        {
            coordinate = DecompositionUtils::getLog10Value(coordinate);
        }

        buffer[offset + 2] = (float)(coordinate * scale[2] + translation[2]);
    }

    if ((elementsSize == 4) && (coordinateMask & 0x08))
    {
        buffer[offset + 3] = 1.0;
    }

}

void PolylineDecomposer::fillStairDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    int closed = 0;
    int* piClosed = &closed;

    /* Offsets of the left and right vertices (respectively) */
    int v0 = 0;
    int v1 = 0;

    int componentIndices[3];

    if (nPoints == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    for (int i = 0; i < nPoints - 1; i++)
    {
        v0 = elementsSize * 2 * i;
        v1 = elementsSize * (2 * i + 1);

        componentIndices[0] = i;
        componentIndices[1] = i;
        componentIndices[2] = i;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation, logMask);

        componentIndices[0] = i + 1;
        componentIndices[1] = i;
        componentIndices[2] = i;

        /* To be optimized: the y and z components are fetched and transformed twice */
        getAndWriteVertexToBuffer(buffer, v1, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation, logMask);
    }

    /* Last point */
    v0 = elementsSize * 2 * (nPoints - 1);

    componentIndices[0] = nPoints - 1;
    componentIndices[1] = nPoints - 1;
    componentIndices[2] = nPoints - 1;

    getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation, logMask);

    /*
     * One additional vertex if closed
     * Its x-coordinate is equal to the one of the polyline's first point
     * whereas its y and z coordinates are equal to those of the last point.
     */
    if (closed)
    {
        v0 = elementsSize * (2 * nPoints - 1);

        componentIndices[0] = 0;
        componentIndices[1] = nPoints - 1;
        componentIndices[2] = nPoints - 1;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation, logMask);
    }

}

void PolylineDecomposer::fillVerticalLinesDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    for (int i = 0; i < nPoints; i++)
    {
        /* Offsets of the lower and upper vertices (respectively) */
        int v0 = 0;
        int v1 = 0;

        /* Coordinates of the lower and upper vertices (respectively) */
        double coord0 = 0.;
        double coord1 = 0.;

        v0 = elementsSize * 2 * i;
        v1 = elementsSize * (2 * i + 1);

        /* Lower and upper endpoints x coordinates */
        if (coordinateMask & 0x01)
        {
            coord0 = coordinates[i];
            coord1 = coordinates[i];

            if (xshift != NULL)
            {
                coord0 += xshift[i];
                coord1 += xshift[i];
            }

            if (logMask & 0x01)
            {
                coord0 = DecompositionUtils::getLog10Value(coord0);
                coord1 = DecompositionUtils::getLog10Value(coord1);
            }

            buffer[v0 + 0] = (float)(coord0 * scale[0] + translation[0]);
            buffer[v1 + 0] = (float)(coord1 * scale[0] + translation[0]);
        }

        /* Lower and upper endpoints y coordinates */
        if (coordinateMask & 0x02)
        {
            coord0 = 0.0;
            coord1 = coordinates[i + nPoints];

            if (yshift != NULL)
            {
                /*
                 * Only the upper vertex's y coordinate is shifted,
                 * the lower vertex's one remains unchanged.
                 */
                coord1 += yshift[i];
            }

            if (logMask & 0x02)
            {
                /* The lower endpoint's y coordinate is unchanged (it amounts to log10(1), which is 0) */
                coord1 = DecompositionUtils::getLog10Value(coord1);
            }

            buffer[v0 + 1] = (float)(coord0 * scale[1] + translation[1]);
            buffer[v1 + 1] = (float)(coord1 * scale[1] + translation[1]);
        }

        /* Lower and upper endpoints z coordinates */
        if (coordinateMask & 0x04)
        {
            coord0 = coordinates[2 * nPoints + i];
            coord1 = coordinates[2 * nPoints + i];

            if (zshift != NULL)
            {
                coord0 += zshift[i];
                coord1 += zshift[i];
            }

            if (logMask & 0x04)
            {
                coord0 = DecompositionUtils::getLog10Value(coord0);
                coord1 = DecompositionUtils::getLog10Value(coord1);
            }

            buffer[v0 + 2] = (float)(coord0 * scale[2] + translation[2]);
            buffer[v1 + 2] = (float)(coord1 * scale[2] + translation[2]);
        }

        if ((elementsSize == 4) && (coordinateMask & 0x08))
        {
            buffer[v0 + 3] = 1.0;
            buffer[v1 + 3] = 1.0;
        }

    }

}

void PolylineDecomposer::writeBarVerticesToBuffer(float* buffer, int* offsets, int componentOffset, double* coordinates, double shift, int shiftUsed,
        double scale, double translation, int logUsed)
{
    if (shiftUsed)
    {
        coordinates[0] += shift;
        coordinates[1] += shift;
        coordinates[2] += shift;
        coordinates[3] += shift;

        coordinates[4] += shift;
    }

    if (logUsed)
    {
        coordinates[0] = DecompositionUtils::getLog10Value(coordinates[0]);
        coordinates[1] = DecompositionUtils::getLog10Value(coordinates[1]);
        coordinates[2] = DecompositionUtils::getLog10Value(coordinates[2]);
        coordinates[3] = DecompositionUtils::getLog10Value(coordinates[3]);

        coordinates[4] = DecompositionUtils::getLog10Value(coordinates[4]);
    }

    buffer[offsets[0] + componentOffset] = (float)(coordinates[0] * scale + translation);
    buffer[offsets[1] + componentOffset] = (float)(coordinates[1] * scale + translation);
    buffer[offsets[2] + componentOffset] = (float)(coordinates[2] * scale + translation);
    buffer[offsets[3] + componentOffset] = (float)(coordinates[3] * scale + translation);

    buffer[offsets[4] + componentOffset] = (float)(coordinates[4] * scale + translation);
}

void PolylineDecomposer::fillVerticalBarsDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    double barWidth = 0.0;
    double* pdBarWidth = &barWidth;

    int shiftUsed[3];
    int *piShiftUsed = NULL;

    /*
     * Offsets of the lower-left, lower-right, upper-right and upper-left bar vertices (respectively)
     * and of the polyline vertex proper
     */
    int offsets[5];

    /*
     * {x,y or z}-component values of a bar's 4 vertices (same ordering as the offsets)
     * and of the polyline vertex proper.
     */
    double coords[5];

    double shift = 0.;


    getGraphicObjectProperty(id, __GO_BAR_WIDTH__, jni_double, (void**) &pdBarWidth);

    piShiftUsed = &shiftUsed[0];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);
    piShiftUsed = &shiftUsed[1];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);
    piShiftUsed = &shiftUsed[2];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);


    for (int i = 0; i < nPoints; i++)
    {
        offsets[0] = elementsSize * 4 * i;
        offsets[1] = elementsSize * (4 * i + 1);
        offsets[2] = elementsSize * (4 * i + 2);
        offsets[3] = elementsSize * (4 * i + 3);

        offsets[4] = elementsSize * (4 * nPoints + i);

        if (coordinateMask & 0x01)
        {
            coords[0] = coordinates[i] - 0.5 * barWidth;
            coords[1] = coordinates[i] + 0.5 * barWidth;
            coords[2] = coordinates[i] + 0.5 * barWidth;
            coords[3] = coordinates[i] - 0.5 * barWidth;

            coords[4] = coordinates[i];

            if (shiftUsed[0])
            {
                shift = xshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 0, coords, shift, shiftUsed[0], scale[0], translation[0], logMask & 0x01);
        }

        if (coordinateMask & 0x02)
        {
            coords[0] = 0.0;
            coords[1] = 0.0;
            coords[2] = coordinates[i + nPoints];
            coords[3] = coordinates[i + nPoints];

            coords[4] = coordinates[i + nPoints];

            if (shiftUsed[1])
            {
                shift = yshift[i];
            }

            if (logMask & 0x02)
            {
                /*
                 * The two lower endpoints' y coordinates must be set to 1
                 * since writeBarVerticesToBuffer applies the logarithmic transformation.
                 */
                coords[0] = 1.0;
                coords[1] = 1.0;
            }

            writeBarVerticesToBuffer(buffer, offsets, 1, coords, shift, shiftUsed[1], scale[1], translation[1], logMask & 0x02);
        }

        if (coordinateMask & 0x04)
        {
            coords[0] = coordinates[i + 2 * nPoints];
            coords[1] = coordinates[i + 2 * nPoints];
            coords[2] = coordinates[i + 2 * nPoints];
            coords[3] = coordinates[i + 2 * nPoints];

            coords[4] = coordinates[i + 2 * nPoints];

            if (shiftUsed[2])
            {
                shift = zshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 2, coords, shift, shiftUsed[2], scale[2], translation[2], logMask & 0x04);
        }

        if ((elementsSize == 4) && (coordinateMask & 0x08))
        {
            buffer[offsets[0] + 3] = 1.0;
            buffer[offsets[1] + 3] = 1.0;
            buffer[offsets[2] + 3] = 1.0;
            buffer[offsets[3] + 3] = 1.0;

            buffer[offsets[4] + 3] = 1.0;
        }

    }

}

/*
 * To do: -refactor with fillVerticalBarsDecompositionVertices as these two functions are very similar, possibly by implementing
 a PolylineBarDecomposer class.
*/
void PolylineDecomposer::fillHorizontalBarsDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    double barWidth = 0.0;
    double* pdBarWidth = &barWidth;

    int shiftUsed[3];
    int *piShiftUsed = NULL;

    /*
     * Offsets of the lower-left, lower-right, upper-right and upper-left bar vertices (respectively)
     * and of the polyline vertex proper
     */
    int offsets[5];

    /*
     * {x,y or z}-component values of a bar's 4 vertices (same ordering as the offsets)
     * and of the polyline vertex proper.
     */
    double coords[5];

    double shift = 0.;


    getGraphicObjectProperty(id, __GO_BAR_WIDTH__, jni_double, (void**) &pdBarWidth);

    piShiftUsed = &shiftUsed[0];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);
    piShiftUsed = &shiftUsed[1];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);
    piShiftUsed = &shiftUsed[2];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);

    for (int i = 0; i < nPoints; i++)
    {
        offsets[0] = elementsSize * 4 * i;
        offsets[1] = elementsSize * (4 * i + 1);
        offsets[2] = elementsSize * (4 * i + 2);
        offsets[3] = elementsSize * (4 * i + 3);

        offsets[4] = elementsSize * (4 * nPoints + i);

        /* The actual x coordinates correspond to the polyline's y coordinates. */
        if (coordinateMask & 0x01)
        {
            coords[0] = 0.0;
            coords[1] = 0.0;
            coords[2] = coordinates[i + nPoints];
            coords[3] = coordinates[i + nPoints];

            coords[4] = coordinates[i];

            if (shiftUsed[1])
            {
                shift = yshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 0, coords, shift, shiftUsed[1], scale[0], translation[0], logMask & 0x01);
        }

        /* The actual y coordinates correspond to the polyline's x coordinates. */
        if (coordinateMask & 0x02)
        {
            coords[0] = coordinates[i] - 0.5 * barWidth;
            coords[1] = coordinates[i] + 0.5 * barWidth;
            coords[2] = coordinates[i] + 0.5 * barWidth;
            coords[3] = coordinates[i] - 0.5 * barWidth;

            coords[4] = coordinates[i + nPoints];

            if (shiftUsed[0])
            {
                shift = xshift[i];
            }

            if (logMask & 0x02)
            {
                /*
                 * The two lower endpoints' y coordinates must be set to 1
                 * since writeBarVerticesToBuffer applies the logarithmic transformation.
                 */
                coords[0] = 1.0;
                coords[1] = 1.0;
            }

            writeBarVerticesToBuffer(buffer, offsets, 1, coords, shift, shiftUsed[0], scale[1], translation[1], logMask & 0x02);
        }

        if (coordinateMask & 0x04)
        {
            coords[0] = coordinates[i + 2 * nPoints];
            coords[1] = coordinates[i + 2 * nPoints];
            coords[2] = coordinates[i + 2 * nPoints];
            coords[3] = coordinates[i + 2 * nPoints];

            coords[4] = coordinates[i + 2 * nPoints];

            if (shiftUsed[2])
            {
                shift = zshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 2, coords, shift, shiftUsed[2], scale[2], translation[2], logMask & 0x04);
        }

        if ((elementsSize == 4) && (coordinateMask & 0x08))
        {
            buffer[offsets[0] + 3] = 1.0;
            buffer[offsets[1] + 3] = 1.0;
            buffer[offsets[2] + 3] = 1.0;
            buffer[offsets[3] + 3] = 1.0;

            buffer[offsets[4] + 3] = 1.0;
        }

    }

}

/*
 * To do:
 *  -implement for the other relevant polyline style values.
 *  -fix the no colors written problem (related to polyline C build functions, see below).
 */
void PolylineDecomposer::fillColors(int id, float* buffer, int bufferLength, int elementsSize)
{
    int parent = 0;
    int* pparent = &parent;
    int parentFigure = 0;
    int* pparentFigure = &parentFigure;

    int interpColorMode = 0;
    int* piInterpColorMode = &interpColorMode;
    int colorSet = 0;
    int* piColorSet = &colorSet;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;
    int bufferOffset = 0;
    int* interpColorVector = NULL;

    double* colormap = NULL;

    getGraphicObjectProperty(id, __GO_INTERP_COLOR_MODE__, jni_bool, (void**) &piInterpColorMode);
    getGraphicObjectProperty(id, __GO_COLOR_SET__, jni_bool, (void**) &piColorSet);

    if (interpColorMode == 0 && colorSet == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);

    if (polylineStyle != 1 && colorSet == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    parent = getParentObject(id);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (parent == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &pparentFigure);

    /*
     * In some cases, the polyline's parent figure may be unitialized, when this point is reached from the
     * filled polygons build C functions (xfpolys, with several polygons and a color vector for each one).
     * This check prevents from crashing when getting the colormap. However, it results in incorrectly
     * black-filled polygons, as no colors are written.
     * As the sequentially built polygons are inserted within a Compound object, the latter object may be
     * still unattached to a Figure as its Polyline children are rendered. This occurs about once in 5 to 10,
     * hence possibly caused by a race condition.
     * To be fixed.
     */
    if (parentFigure == 0)
    {
        return;
    }

    if (interpColorMode == 1)
    {
        /*
         * The interpolated color vector is a 3- or 4-element vector.
         * However, if nPoints is greater than 4, we choose to output
         * 4 colors (this behaviour is kept for compatibility, see fillTriangleIndices).
         */
        if (nPoints < 3)
        {
            return;
        }

        getGraphicObjectProperty(id, __GO_INTERP_COLOR_VECTOR__, jni_int_vector, (void**) &interpColorVector);
        getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
        getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

        if (nPoints > 4)
        {
            nPoints = 4;
        }

        for (int i = 0; i < nPoints; i++)
        {
            ColorComputer::getDirectColor((double) interpColorVector[i] - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset + 3] = 1.0;
            }

            bufferOffset += elementsSize;
        }

        releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
        releaseGraphicObjectProperty(__GO_INTERP_COLOR_VECTOR__, interpColorVector, jni_int_vector, 0);
    }
    else
    {
        int* colors = NULL;
        int numColors = 0;
        int * piNumColors = &numColors;
        int min;

        getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_COLORS__, jni_int, (void**) &piNumColors);
        if (numColors > 0)
        {
            getGraphicObjectProperty(id, __GO_DATA_MODEL_COLORS__, jni_int_vector, (void**) &colors);
        }

        if (numColors == 0 || colors == NULL)
        {
            // try to load mark background colors
            getGraphicObjectProperty(id, __GO_NUM_MARK_BACKGROUNDS__, jni_int, (void**) &piNumColors);
            if (numColors > 0)
            {
                getGraphicObjectProperty(id, __GO_MARK_BACKGROUNDS__, jni_int_vector, (void**) &colors);
            }
        }

        if (numColors == 0 || colors == NULL)
        {
            // try to load mark foreground colors
            getGraphicObjectProperty(id, __GO_NUM_MARK_FOREGROUNDS__, jni_int, (void**) &piNumColors);
            if (numColors > 0)
            {
                getGraphicObjectProperty(id, __GO_MARK_FOREGROUNDS__, jni_int_vector, (void**) &colors);
            }
        }

        if (!colors)
        {
            buffer[bufferOffset + 3] = 1.0;
        }

        min = nPoints < numColors ? nPoints : numColors;

        getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
        getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

        for (int i = 0; i < min; i++)
        {
            ColorComputer::getDirectColor(colors[i] - 1.0, colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset + 3] = 1.0;
            }

            bufferOffset += elementsSize;
        }

        releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
        releaseGraphicObjectProperty(__GO_INTERP_COLOR_VECTOR__, interpColorVector, jni_int_vector, 0);
    }
}

void PolylineDecomposer::fillTextureCoordinates(int id, float* buffer, int bufferLength)
{
    int parent = 0;
    int* pparent = &parent;
    int parentFigure = 0;
    int* pparentFigure = &parentFigure;

    int interpColorMode = 0;
    int* piInterpColorMode = &interpColorMode;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;
    int bufferOffset = 0;
    int* interpColorVector = NULL;

    double* colormap = NULL;

    getGraphicObjectProperty(id, __GO_INTERP_COLOR_MODE__, jni_bool, (void**) &piInterpColorMode);

    if (interpColorMode == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);

    if (polylineStyle  != 1)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    parent = getParentObject(id);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (parent == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_int, (void**) &pparentFigure);

    /*
     * In some cases, the polyline's parent figure may be unitialized, when this point is reached from the
     * filled polygons build C functions (xfpolys, with several polygons and a color vector for each one).
     * This check prevents from crashing when getting the colormap. However, it results in incorrectly
     * black-filled polygons, as no colors are written.
     * As the sequentially built polygons are inserted within a Compound object, the latter object may be
     * still unattached to a Figure as its Polyline children are rendered. This occurs about once in 5 to 10,
     * hence possibly caused by a race condition.
     * To be fixed.
     */
    if (parentFigure == 0)
    {
        return;
    }

    /*
     * The interpolated color vector is a 3- or 4-element vector.
     * However, if nPoints is greater than 4, we choose to output
     * 4 colors (this behaviour is kept for compatibility, see fillTriangleIndices).
     */
    if (nPoints < 3)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_INTERP_COLOR_VECTOR__, jni_int_vector, (void**) &interpColorVector);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    if (nPoints > 4)
    {
        nPoints = 4;
    }

    for (int i = 0; i < nPoints; i++)
    {
        double index = (ColorComputer::getDirectIndex((double) interpColorVector[i] - 1.0, colormapSize) + 2.0 + COLOR_TEXTURE_OFFSET) / (double) (colormapSize + 2);

        buffer[bufferOffset] = (float)index;
        buffer[bufferOffset + 1] = 0.0;
        buffer[bufferOffset + 2] = 0.0;
        buffer[bufferOffset + 3] = 1.0;

        bufferOffset += 4;
    }

    releaseGraphicObjectProperty(__GO_COLORMAP__, colormap, jni_double_vector, colormapSize);
    releaseGraphicObjectProperty(__GO_INTERP_COLOR_VECTOR__, interpColorVector, jni_int_vector, 0);
}

/*
 * To do: see fillIndices
 * -take into account polyline_style.
 */
int PolylineDecomposer::getIndicesSize(int id)
{
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int closed = 0;
    int* piClosed = &closed;

    int nIndices = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    /* No facets if 0 points */
    if (nPoints == 0)
    {
        return 0;
    }

    /* Segments */
    if (polylineStyle == 1)
    {
        if (nPoints < 3)
        {
            return 0;
        }

        /* Maximum number of triangles output by the triangulator */
        nIndices = 3 * (nPoints - 2);
    }
    /* Arrowed segments */
    else if (polylineStyle == 4)
    {
        nIndices = PolylineDecomposer::getArrowTriangleIndicesSize(nPoints, closed);
    }
    /* Filled patch  */
    else if (polylineStyle == 5)
    {
        if (nPoints < 3)
        {
            return 0;
        }

        /* Maximum number of triangles output by the triangulator */
        nIndices = 3 * (nPoints - 2);
    }
    /* Vertical bars plus segments */
    else if (polylineStyle == 6)
    {
        nIndices = PolylineDecomposer::getBarsDecompositionTriangleIndicesSize(nPoints);
    }
    /* Horizontal bars plus segments */
    else if (polylineStyle == 7)
    {
        nIndices = PolylineDecomposer::getBarsDecompositionTriangleIndicesSize(nPoints);
    }

    return nIndices;
}

int PolylineDecomposer::getArrowTriangleIndicesSize(int nPoints, int closed)
{
    int nIndices = 0;

    if (nPoints < 2)
    {
        nIndices = 0;
    }
    else
    {
        nIndices = 3 * (nPoints - 1);

        if (closed)
        {
            nIndices += 3;
        }
    }

    return nIndices;
}

int PolylineDecomposer::getBarsDecompositionTriangleIndicesSize(int nPoints)
{
    return 2 * 3 * nPoints;
}

/*
 * To do:
 * -take into account the polyline style property (note: vertical bars -style 6- are filled
 *  whatever fill_mode's value), by implementing the relevant functions (see fillTriangleIndices),
 *  as the curve must be filled if fill_mode set to on, whatever its polyline style value.
 */
int PolylineDecomposer::fillIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* coordinates = NULL;
    double* xshift = NULL;
    double* yshift = NULL;
    double* zshift = NULL;

    int nPoints = 0;
    int* piNPoints = &nPoints;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int fillMode = 0;
    int* piFillMode = &fillMode;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &coordinates);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void**) &xshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, (void**) &yshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double_vector, (void**) &zshift);

    getGraphicObjectProperty(id, __GO_FILL_MODE__, jni_bool, (void**) &piFillMode);

    /* 0 triangles if 0 points */
    if (nPoints == 0)
    {
        return 0;
    }

    switch (polylineStyle)
    {
        case 1 :
            return fillTriangleIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, fillMode, polylineStyle);
        case 4 :
            return fillArrowTriangleIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift);
        case 5 :
            /* Set fill mode to on, since patches are always filled whatever fill mode's value */
            return fillTriangleIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, 1, polylineStyle);
        case 6 :
            return fillBarsDecompositionTriangleIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift);
        case 7 :
            return fillBarsDecompositionTriangleIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift);
    }

    return 0;
}

int PolylineDecomposer::fillTriangleIndices(int id, int* buffer, int bufferLength,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int fillMode, int polylineStyle)
{
    double coords[4][3];

    int interpColorMode = 0;
    int* piInterpColorMode = &interpColorMode;
    int triangulate = 0;

    int isValid = 0;
    int tmpValid = 0;
    int nIndices = 0;

    /* At least 3 points needed */
    if (nPoints < 3)
    {
        return 0;
    }

    if (fillMode == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_INTERP_COLOR_MODE__, jni_bool, (void**) &piInterpColorMode);

    /*
     * Do not triangulate if the interpolated color mode is set to 'on' and the polyline style is filled patch (5).
     * The quadrilateral facet decomposition function is used instead, if nPoints == 4,
     * for compatibility reasons, although triangulation could be used.
     */
    if (interpColorMode && polylineStyle != 5)
    {
        triangulate = 0;
    }
    else if (nPoints > 3)
    {
        /* Perform triangulation only if more than 3 points */
        triangulate = 1;
    }

    if (triangulate)
    {
        Triangulator triangulator;
        int numTriangles;
        int* indices;

        isValid = 1;

        for (int i = 0; i < nPoints; i++)
        {
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i, &coords[0][0], &coords[0][1], &coords[0][2]);

            tmpValid = DecompositionUtils::isValid(coords[0][0], coords[0][1], coords[0][2]);

            if (logMask)
            {
                tmpValid &= DecompositionUtils::isLogValid(coords[0][0], coords[0][1], coords[0][2], logMask);

                if (logMask & 0x01)
                {
                    coords[0][0] = DecompositionUtils::getLog10Value(coords[0][0]);
                }

                if (logMask & 0x02)
                {
                    coords[0][1] = DecompositionUtils::getLog10Value(coords[0][1]);
                }

                if (logMask & 0x04)
                {
                    coords[0][2] = DecompositionUtils::getLog10Value(coords[0][2]);
                }
            }

            isValid &= tmpValid;

            if (!isValid)
            {
                break;
            }

            triangulator.addPoint(coords[0][0], coords[0][1], coords[0][2]);
        }

        if (!isValid)
        {
            return 0;
        }

        /* Triangulate */
        triangulator.initialize();
        triangulator.triangulate();

        numTriangles = triangulator.getNumberTriangles();
        indices = triangulator.getIndices();

        for (int i = 0; i < numTriangles; i++)
        {
            buffer[3 * i] = indices[3 * i];
            buffer[3 * i + 1] = indices[3 * i + 1];
            buffer[3 * i + 2] = indices[3 * i + 2];
            nIndices += 3;
        }

        triangulator.clear();

        return nIndices;
    }
    else
    {
        /* Do not triangulate: either the interpolation color mode is set to on or it is not and there are only 3 points. */

        /* 3 points: only one triangle output */
        if (nPoints == 3)
        {
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coords[0][0], &coords[0][1], &coords[0][2]);
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 1, &coords[1][0], &coords[1][1], &coords[1][2]);
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 2, &coords[2][0], &coords[2][1], &coords[2][2]);

            tmpValid = DecompositionUtils::isValid(coords[0][0], coords[0][1], coords[0][2]);
            tmpValid &= DecompositionUtils::isValid(coords[1][0], coords[1][1], coords[1][2]);
            tmpValid &= DecompositionUtils::isValid(coords[2][0], coords[2][1], coords[2][2]);

            isValid = tmpValid;

            if (logMask)
            {
                tmpValid = DecompositionUtils::isLogValid(coords[0][0], coords[0][1], coords[0][2], logMask);
                tmpValid &= DecompositionUtils::isLogValid(coords[1][0], coords[1][1], coords[1][2], logMask);
                tmpValid &= DecompositionUtils::isLogValid(coords[2][0], coords[2][1], coords[2][2], logMask);
                isValid &= tmpValid;
            }

            if (isValid)
            {
                buffer[0] = 0;
                buffer[1] = 1;
                buffer[2] = 2;

                nIndices += 3;
            }
        }
        else if (nPoints >= 4)
        {
            /*
             * 4 points: the quadrilateral facet decomposition algorithm is used.
             * If the Polyline has more than 4 points, we still output two triangles
             * corresponding to the first 4 points. This behaviour is kept for compatibility.
             * Possible correction: do not output indices if there are more than 4 points and
             * the interpolation color mode is set to on.
             */
            int facetVertexIndices[4] = {0, 1, 2, 3};

            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coords[0][0], &coords[0][1], &coords[0][2]);
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 1, &coords[1][0], &coords[1][1], &coords[1][2]);
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 2, &coords[2][0], &coords[2][1], &coords[2][2]);
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 3, &coords[3][0], &coords[3][1], &coords[3][2]);

            tmpValid = DecompositionUtils::isValid(coords[0][0], coords[0][1], coords[0][2]);
            tmpValid &= DecompositionUtils::isValid(coords[1][0], coords[1][1], coords[1][2]);
            tmpValid &= DecompositionUtils::isValid(coords[2][0], coords[2][1], coords[2][2]);
            tmpValid &= DecompositionUtils::isValid(coords[3][0], coords[3][1], coords[3][2]);

            isValid = tmpValid;

            if (logMask)
            {
                tmpValid = DecompositionUtils::isLogValid(coords[0][0], coords[0][1], coords[0][2], logMask);
                tmpValid &= DecompositionUtils::isLogValid(coords[1][0], coords[1][1], coords[1][2], logMask);
                tmpValid &= DecompositionUtils::isLogValid(coords[2][0], coords[2][1], coords[2][2], logMask);
                tmpValid &= DecompositionUtils::isLogValid(coords[3][0], coords[3][1], coords[3][2], logMask);
                isValid &= tmpValid;
            }

            if (isValid)
            {
                DecompositionUtils::getDecomposedQuadTriangleIndices(coords, facetVertexIndices, buffer);
                nIndices += 6;
            }
        }

    }

    return nIndices;
}

int PolylineDecomposer::fillArrowTriangleIndices(int id, int* buffer, int bufferLength,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    int closed = 0;
    int* piClosed = &closed;

    int currentValid = 0;
    int nextValid = 0;

    int firstArrowVertex = 0;
    int nArrows = 0;

    int offset = 0;
    int numberValidIndices = 0;

    /* At least 2 points needed to form segments */
    if (nPoints < 2)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    /* If closed, an additional segment is present */
    if (closed)
    {
        nArrows = nPoints;
    }
    else
    {
        nArrows = nPoints - 1;
    }

    /*
     * Arrow head vertices are stored consecutively after all the line vertices.
     * Hence the offset to the first arrow vertex.
     */
    firstArrowVertex = nPoints;

    for (int i = 0; i < nArrows; i++)
    {
        /* Indices of the tip, left and right vertices */
        int n = 3 * i;
        buffer[n] = firstArrowVertex + n;
        buffer[n + 1] = buffer[n] + 1;
        buffer[n + 2] = buffer[n] + 2;
    }

    return 3 * nArrows;
}

/*
 * Only bars are filled at the present moment, the curve is not.
 * See fillTriangleIndices for more information.
 */
int PolylineDecomposer::fillBarsDecompositionTriangleIndices(int id, int* buffer, int bufferLength,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    double barWidth = 0.0;
    double* pdBarWidth = &barWidth;
    double coordsi[3];

    int triangleIndices[6];
    int offset = 0;
    int numberValidIndices = 0;

    if (nPoints == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_BAR_WIDTH__, jni_double, (void**) &pdBarWidth);

    /* 0 indices if the bar width is invalid, as it is the same for all bars. */
    if (!DecompositionUtils::isValid(barWidth))
    {
        return 0;
    }

    /*
     * Gets the indices corresponding to a rectangle decomposed into 2 triangles.
     * All the bars are decomposed the same way.
     */
    DecompositionUtils::getDecomposedRectangleTriangleIndices(triangleIndices);

    /* Bars */
    for (int i = 0; i < nPoints; i++)
    {
        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i, &coordsi[0], &coordsi[1], &coordsi[2]);

        if (DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]))
        {
            if (logMask && !DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask))
            {
                continue;
            }

            buffer[6 * offset] = 4 * i + triangleIndices[0];
            buffer[6 * offset + 1] = 4 * i + triangleIndices[1];
            buffer[6 * offset + 2] = 4 * i + triangleIndices[2];
            buffer[6 * offset + 3] = 4 * i + triangleIndices[3];
            buffer[6 * offset + 4] = 4 * i + triangleIndices[4];
            buffer[6 * offset + 5] = 4 * i + triangleIndices[5];

            numberValidIndices += 6;
            offset++;
        }

    }

    return numberValidIndices;
}

int PolylineDecomposer::getWireIndicesSize(int id)
{
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;

    int lineMode = 0;
    int* piLineMode = &lineMode;

    int closed = 0;
    int* piClosed = &closed;

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_LINE_MODE__, jni_bool, (void**) &piLineMode);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    /* No segments if 0 points */
    if (nPoints == 0)
    {
        return 0;
    }

    switch (polylineStyle)
    {
        case 1:
            /* Segments */
            return getSegmentsDecompositionSegmentIndicesSize(nPoints, lineMode, closed);
        case 2 :
            /* Staircase */
            return getStairDecompositionSegmentIndicesSize(nPoints, lineMode, closed);
        case 3 :
            /* Vertical segments plus segments */
            return getVerticalLinesDecompositionSegmentIndicesSize(nPoints, lineMode);
        case 4 :
            /* Segments with arrow heads */
            return getSegmentsDecompositionSegmentIndicesSize(nPoints, lineMode, closed);
        case 5 :
            /* Filled patch  */
            return getSegmentsDecompositionSegmentIndicesSize(nPoints, lineMode, closed);
        case 6 :
            /* Vertical bars plus segments */
            return getBarsDecompositionSegmentIndicesSize(nPoints, lineMode);
        case 7 :
            /* Horizontal bars plus segments */
            return getBarsDecompositionSegmentIndicesSize(nPoints, lineMode);
    }

    return 0;
}

int PolylineDecomposer::getSegmentsDecompositionSegmentIndicesSize(int nPoints, int lineMode, int closed)
{
    if (nPoints < 2)
    {
        return 0;
    }

    if (lineMode)
    {
        if (closed)
        {
            return nPoints + 1;
        }
        else
        {
            return nPoints;
        }
    }
    else
    {
        return 0;
    }
}

int PolylineDecomposer::getStairDecompositionSegmentIndicesSize(int nPoints, int lineMode, int closed)
{
    if (nPoints < 2)
    {
        return 0;
    }

    if (lineMode)
    {
        if (closed)
        {
            return 2 * nPoints + 1;
        }
        else
        {
            return 2 * nPoints - 1;
        }
    }
    else
    {
        return 0;
    }
}

int PolylineDecomposer::getVerticalLinesDecompositionSegmentIndicesSize(int nPoints, int lineMode)
{
    if (nPoints == 0)
    {
        return 0;
    }

    if (lineMode)
    {
        return 2 * (nPoints) + 2 * (nPoints - 1);
    }
    else
    {
        return 2 * (nPoints);
    }
}

int PolylineDecomposer::getBarsDecompositionSegmentIndicesSize(int nPoints, int lineMode)
{
    if (nPoints == 0)
    {
        return 0;
    }

    if (lineMode)
    {
        return 2 * 4 * (nPoints) + 2 * (nPoints - 1);
    }
    else
    {
        return 2 * 4 * (nPoints);
    }
}

int PolylineDecomposer::fillWireIndices(int id, int* buffer, int bufferLength, int logMask)
{
    double* coordinates = NULL;
    double* xshift = NULL;
    double* yshift = NULL;
    double* zshift = NULL;

    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int nPoints = 0;
    int* piNPoints = &nPoints;
    int closed = 0;
    int* piClosed = &closed;
    int lineMode = 0;
    int* piLineMode = &lineMode;

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &coordinates);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void**) &xshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, (void**) &yshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double_vector, (void**) &zshift);

    getGraphicObjectProperty(id, __GO_LINE_MODE__, jni_bool, (void**) &piLineMode);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    switch (polylineStyle)
    {
        case 1 :
            return fillSegmentsDecompositionSegmentIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode, closed);
        case 2 :
            return fillStairDecompositionSegmentIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode, closed);
        case 3 :
            return fillVerticalLinesDecompositionSegmentIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode);
        case 4 :
            return fillSegmentsDecompositionSegmentIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode, closed);
        case 5 :
            return fillSegmentsDecompositionSegmentIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode, closed);
        case 6 :
            return fillBarsDecompositionSegmentIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode);
        case 7 :
            return fillBarsDecompositionSegmentIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode);
    }

    return 0;
}

int PolylineDecomposer::fillSegmentsDecompositionSegmentIndices(int id, int* buffer, int bufferLength,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode, int closed)
{
    /* If less than 2 points, no segments */
    if (nPoints < 2)
    {
        return 0;
    }

    if (lineMode == 0)
    {
        return 0;
    }

    for (int i = 0; i < nPoints; i++)
    {
        buffer[i] = i;
    }

    if (closed)
    {
        buffer[nPoints] = 0;
    }

    return closed ? (nPoints + 1) : nPoints;
}

int PolylineDecomposer::fillStairDecompositionSegmentIndices(int id, int* buffer, int bufferLength,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode, int closed)
{
    int currentValid = 0;
    int middleVertexValid = 0;
    int nextValid = 0;

    int offset = 0;
    int numberValidIndices = 0;

    /* If less than 2 points, no segments */
    if (nPoints < 2)
    {
        return 0;
    }

    if (lineMode == 0)
    {
        return 0;
    }

    for (int i = 0; i < 2 * nPoints - 1; i++)
    {
        buffer[i] = i;
    }

    if (closed)
    {
        buffer[2 * nPoints - 1] = 2 * nPoints - 1;
        buffer[2 * nPoints] = 0;
    }

    return closed ? (2 * nPoints + 1) : (2 * nPoints - 1);
}

int PolylineDecomposer::fillVerticalLinesDecompositionSegmentIndices(int id, int* buffer, int bufferLength,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode)
{
    double coordsi[3];

    int offset = 0;
    int numberValidIndices = 0;

    if (nPoints == 0)
    {
        return 0;
    }

    /* Vertical lines */
    for (int i = 0; i < nPoints; i++)
    {
        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i, &coordsi[0], &coordsi[1], &coordsi[2]);

        if (DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]))
        {
            if (logMask && !DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask))
            {
                continue;
            }

            buffer[2 * offset] = 2 * i;
            buffer[2 * offset + 1] = 2 * i + 1;

            numberValidIndices += 2;
            offset++;
        }

    }

    if (lineMode)
    {
        int currentValid;
        int nextValid;

        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coordsi[0], &coordsi[1], &coordsi[2]);

        currentValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

        if (logMask)
        {
            currentValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
        }

        for (int i = 0; i < nPoints - 1; i++)
        {
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i + 1, &coordsi[0], &coordsi[1], &coordsi[2]);

            nextValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

            if (logMask)
            {
                nextValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
            }

            if (currentValid && nextValid)
            {
                buffer[2 * offset] = 2 * i + 1;
                buffer[2 * offset + 1] = 2 * (i + 1) + 1;

                numberValidIndices += 2;
                offset++;
            }

            currentValid = nextValid;
        }
    }

    return numberValidIndices;
}


int PolylineDecomposer::fillBarsDecompositionSegmentIndices(int id, int* buffer, int bufferLength,
        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode)
{
    double barWidth = 0.0;
    double* pdBarWidth = &barWidth;
    double coordsi[3];

    int barWidthValid = 0;
    int offset = 0;
    int numberValidIndices = 0;

    if (nPoints == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_BAR_WIDTH__, jni_double, (void**) &pdBarWidth);

    barWidthValid = DecompositionUtils::isValid(barWidth);

    /* 0 bar segment indices if the bar width is invalid, as it is the same for all bars. */
    if (barWidthValid)
    {
        /* Bars */
        for (int i = 0; i < nPoints; i++)
        {
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i, &coordsi[0], &coordsi[1], &coordsi[2]);

            if (DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]))
            {
                if (logMask && !DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask))
                {
                    continue;
                }

                buffer[8 * offset] = 4 * i;
                buffer[8 * offset + 1] = 4 * i + 1;
                buffer[8 * offset + 2] = 4 * i + 1;
                buffer[8 * offset + 3] = 4 * i + 2;
                buffer[8 * offset + 4] = 4 * i + 2;
                buffer[8 * offset + 5] = 4 * i + 3;
                buffer[8 * offset + 6] = 4 * i + 3;
                buffer[8 * offset + 7] = 4 * i;

                numberValidIndices += 8;
                offset++;
            }

        }

    }

    /* Lines */
    if (lineMode)
    {
        int loffset = 0;

        int currentValid;
        int nextValid;

        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coordsi[0], &coordsi[1], &coordsi[2]);

        currentValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

        if (logMask)
        {
            currentValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
        }

        for (int i = 0; i < nPoints - 1; i++)
        {
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i + 1, &coordsi[0], &coordsi[1], &coordsi[2]);

            nextValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

            if (logMask)
            {
                nextValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
            }

            if (currentValid && nextValid)
            {
                buffer[8 * offset + 2 * loffset] = 4 * nPoints + i;
                buffer[8 * offset + 2 * loffset + 1] = 4 * nPoints + i + 1;

                numberValidIndices += 2;
                loffset++;
            }

            currentValid = nextValid;
        }
    }

    return numberValidIndices;
}

void PolylineDecomposer::getShiftedPolylinePoint(double* coordinates, double* xshift, double* yshift, double* zshift, int nPoints, int index,
        double* x, double* y, double* z)
{
    *x = coordinates[index];

    if (xshift != NULL)
    {
        *x += xshift[index];
    }

    *y = coordinates[index + nPoints];

    if (yshift != NULL)
    {
        *y += yshift[index];
    }

    *z = coordinates[index + 2 * nPoints];

    if (zshift != NULL)
    {
        *z += zshift[index];
    }
}
