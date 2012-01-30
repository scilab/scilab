/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef NGONGRIDMATPLOTDATA_DECOMPOSER_HXX
#define NGONGRIDMATPLOTDATA_DECOMPOSER_HXX

#include <string>

/**
 * NgonGridMatplotData decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed Matplot object's properties.
 *
 */

class NgonGridMatplotDataDecomposer : public NgonGridDataDecomposer
{

private :

    /** NgonGridMatplotData decomposer instance */
    static NgonGridMatplotDataDecomposer* decomposer;

protected :

    /**
     * Determines whether a facet is valid.
     * The facet is identified by its lower left-corner (i,j).
     * For a Matplot object, facet (i,j)'s validity depends only on its z coordinate and its (i,j) value,
     * edge validity being therefore irrelevant.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used. Unused.
     * @param[in] (i,j) to (i,j+1) edge validity flag. Unused.
     * @param[out] a pointer to the output (i+1,j) to (i+1,j+1) edge validity flag. Always set to 1.
     * @return 1 if the facet is valid, 0 if it is not.
     */
    virtual int isFacetValid(double* z, double* values, int numX, int numY, int i, int j, int logUsed, int currentEdgeValid, int* nextEdgeValid);

    /**
     * Determines whether the left edge of a facet is valid.
     * For a Matplot object, edge validity is irrelevant, it is therefore always equal to 1.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @return Always 1.
     */
    virtual int isFacetEdgeValid(double* z, double* values, int numX, int numY, int i, int j, int logUsed);

public :

    /**
     * Returns the class single instance.
     * @return the class instance.
     */
    static NgonGridMatplotDataDecomposer* get(void)
    {
        if (decomposer == NULL)
        {
            decomposer = new NgonGridMatplotDataDecomposer();
        }

        return decomposer;
    }

    /**
     * Fills the given buffer with color data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of components taken by one element in the buffer (3 or 4).
     */
    static void fillColors(char* id, float* buffer, int bufferLength, int elementsSize);

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
