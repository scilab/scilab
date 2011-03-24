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

#ifndef NGONGRIDDATA_DECOMPOSER_H
#define NGONGRIDDATA_DECOMPOSER_H

#include <string>

extern "C" {
#include <stdlib.h>
}

/**
 * NgonGridData decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed NgonGridData object's properties.
 */

class NgonGridDataDecomposer
{

private :

    /** NgonGridData decomposer instance */
    static NgonGridDataDecomposer* decomposer;

protected :

    /**
     * Fills a buffer with vertex data from a grid.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation factor to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the grid x-coordinate array.
     * @param[in] the grid y-coordinate array.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     */
    void fillGridVertices(float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask,
        double* x, double* y, double* z, int numX, int numY);

    /**
     * Returns the z-coordinate of the (i,j) grid point.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of points along the x-axis.
     * @param[in] the grid's number of points along the y-axis.
     * @param[in] the point's x index.
     * @param[in] the point's y index.
     * @return the (i,j) grid point's z-coordinate.
     */
    virtual double getZCoordinate(double* z, int numX, int numY, int i, int j);

    /**
     * Returns the z-coordinate of the (i,j) grid point, taking
     * into account logarithmic scaling.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of points along the x-axis.
     * @param[in] the grid's number of points along the y-axis.
     * @param[in] the point's x index.
     * @param[in] the point's y index.
     * @param[in] a flag specifying whether logarithmic scaling is used.
     * @return the (i,j) grid point's z-coordinate.
     */
    virtual double getZCoordinate(double* z, int numX, int numY, int i, int j, int logUsed);

    /**
     * Returns the value of the (i,j) grid point.
     * @param[in] the grid z-value array.
     * @param[in] the grid's number of points along the x-axis.
     * @param[in] the grid's number of points along the y-axis.
     * @param[in] the point's x index.
     * @param[in] the point's y index.
     * @return the (i,j) grid point's value.
     */
    virtual double getZValue(double* z, int numX, int numY, int i, int j);

    /**
     * Fills a buffer with triangle indices from a decomposed grid.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the grid x-coordinate array.
     * @param[in] the grid y-coordinate array.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @return the number of indices actually written.
     */
    int fillTriangleIndices(int* buffer, int bufferLength, int logMask, double* x, double* y, double* z, int numX, int numY);

    /**
     * Determines whether the left edge of a facet is valid.
     * The left edge is between the lower-left corner (i,j) and the
     * upper-left corner (i,j+1).
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @return 1 if the edge valid, 0 if it is not.
     */
    virtual int isFacetEdgeValid(double* z, int numX, int numY, int i, int j, int logUsed);

    /**
     * Returns a 1D vertex index from its x and y indices.
     * @param[in] the number of vertices along the x-axis.
     * @param[in] the number of vertices along the y-axis.
     * @param[in] the x index.
     * @param[in] the y index.
     * @return the 1D vertex index.
     */
    static int getPointIndex(int numX, int numY, int i, int j);

    /**
     * Computes and returns the grid's minimum and maximum z values.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of z values along the x-axis.
     * @param[in] the grid's number of z values along the y-axis.
     * @param[out] a pointer to the returned minimum z value.
     * @param[out] a pointer to the returned maximum z value.
     */
    static void computeMinMaxZValues(double* z, int numX, int numY, double* zMin, double* zMax);

public :

    /**
     * Returns the class' single instance.
     * @return the class instance
     */
    static NgonGridDataDecomposer* get(void)
    {
        if (decomposer == NULL)
        {
            decomposer = new NgonGridDataDecomposer();
        }

        return decomposer;
    }

    /**
     * Returns the number of data elements for the given object.
     * @param[in] the given object id.
     * @return the number of data elements.
     */
    static int getDataSize(char* id);

    /**
     * Fills the given buffer with vertex data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the transformation to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     */
    static void fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask);

    /**
     * Returns the number of indices for the given object.
     * @param[in] the given object id.
     * @return the object's number of indices.
     */
    static int getIndicesSize(char* id);

    /**
     * Fills the given buffer with indices data of the given object.
     * @param[in] the given object id.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    static int fillIndices(char* id, int* buffer, int bufferLength, int logMask);
};

#endif
