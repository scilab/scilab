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

#ifndef PLOT3D_DECOMPOSER_HXX
#define PLOT3D_DECOMPOSER_HXX

#include "NgonGridDataDecomposer.hxx"

/**
 * Plot3D decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed Plot3D object's properties.
 */

class Plot3DDecomposer : public NgonGridDataDecomposer
{

private :

    /** Plot3D decomposer instance */
    static Plot3DDecomposer* decomposer;

protected :

    /**
     * Returns the z-coordinate of the (i,j) grid point.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of points along the x-axis.
     * @param[in] the grid's number of points along the y-axis.
     * @param[in] the point's x index.
     * @param[in] the point's y index.
     * @return the (i,j) grid point's z-coordinate.
     */
    double getZCoordinate(double* z, int numX, int numY, int i, int j);

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
    double getZCoordinate(double* z, int numX, int numY, int i, int j, int logUsed);

    /**
     * Decomposes facet (i,j) into triangles and outputs the resulting vertex indices, where (i,j) is
     * the facet's lower-left corner. As there are two possible decompositions, it chooses the one which
     * has the most coplanar triangles. The output triangles' vertex indices are in counter-clockwise order.
     * @param[in] the grid x-coordinate array.
     * @param[in] the grid y-coordinate array.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] the facet vertices' indices (4-element array: (i,j), (i+1,j), (i+1,j+1) and (i,j+1) indices).
     * @param[out] the triangles' vertex indices (6-element array: two consecutive triplets).
     */
    virtual void getFacetTriangles(double* x, double* y, double* z, int numX, int numY, int i, int j,
                                   int* facetVertexIndices, int* triangleVertexIndices);

public :

    /**
     * Returns the class' single instance.
     * @return the class instance
     */
    static Plot3DDecomposer* get(void)
    {
        if (decomposer == NULL)
        {
            decomposer = new Plot3DDecomposer();
        }

        return decomposer;
    }

    static void destroy()
    {
        if (decomposer)
        {
            delete decomposer;
            decomposer = NULL;
        }
    }
    /**
     * Fills the given buffer with vertex data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation factor to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     */
    static void fillVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask);

    /**
     * Fills the given buffer with color data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of components taken by one element in the buffer (3 or 4).
     */
    static void fillColors(int id, float* buffer, int bufferLength, int elementsSize);

    /**
     * Fills the given buffer with index data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    static int fillIndices(int id, int* buffer, int bufferLength, int logMask);

    /**
     * Returns the number of wireframe indices for the given object.
     * It is equal to twice the number of segments that can be at most
     * drawn according to the Plot3D object's properties and therefore does not
     * take non-representable points into account.
     * @param[in] the given object id.
     * @return the object's number of indices.
     */
    static int getWireIndicesSize(int id);

    /**
     * Fills the given buffer with wireframe indices data of the given object.
     * The number of indices actually written is equal to or less than
     * the buffer length, which is the object's maximum number of indices.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    static int fillWireIndices(int id, int* buffer, int bufferLength, int logMask);
};

#endif
