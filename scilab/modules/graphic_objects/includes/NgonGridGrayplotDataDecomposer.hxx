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

#ifndef NGONGRIDGRAYPLOTDATA_DECOMPOSER_HXX
#define NGONGRIDGRAYPLOTDATA_DECOMPOSER_HXX

#include <string>

#include "NgonGridDataDecomposer.hxx"

extern "C" {
#include <stdlib.h>
}

/**
 * NgonGridGrayplotData decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed Grayplot object's properties.
 */

class NgonGridGrayplotDataDecomposer : public NgonGridDataDecomposer
{

private :

    /** NgonGridGrayplotData decomposer instance */
    static NgonGridGrayplotDataDecomposer* decomposer;

protected :

    /**
     * Determines whether the left edge of a facet is valid.
     * The left edge is between the lower-left corner (i,j) and the
     * upper-left corner (i,j+1). The edge's validity depends on its
     * endpoints' z coordinates and grid values.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @return 1 if the edge is valid, 0 if it is not.
     */
    virtual int isFacetEdgeValid(double* z, double* values, int numX, int numY, int i, int j, int logUsed);

public :

    /**
     * Returns the class single instance.
     * @return the class instance.
     */
    static NgonGridGrayplotDataDecomposer* get(void)
    {
        if (decomposer == NULL)
        {
            decomposer = new NgonGridGrayplotDataDecomposer();
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
