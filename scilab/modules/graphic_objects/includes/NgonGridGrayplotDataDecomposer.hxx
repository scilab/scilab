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
     * Determines whether a facet is valid.
     * The facet is identified by its lower left-corner (i,j). It requires a flag (computed beforehand)
     * as an input which indicates whether the (i,j) to (i,j+1) edge is valid or not, and outputs
     * another flag indicating whether the (i+1,j) to (i+1,j+1) edge is valid or not.
     * Facet validity determination depends on the flag specifying how grid values are defined (per-node or per-facet).
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] a flag indicating whether grid values are defined per node (1) or per facet (0).
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @param[in] a flag indicating whether the (i,j) to (i,j+1) edge is valid.
     * @param[out] a pointer to the output flag indicating whether the (i+1,j) to (i+1,j+1) edge is valid.
     * @return 1 if the facet is valid, 0 if it is not.
     */
    virtual int isFacetValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed, int currentEdgeValid, int* nextEdgeValid);

    /**
     * Determines whether the left edge of a facet is valid.
     * The left edge is between the lower-left corner (i,j) and the
     * upper-left corner (i,j+1). The edge's validity depends on its
     * endpoints' z coordinates and grid values. Its determination also depends on
     * the flag specifying how grid values are defined (per-node or per-facet).
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] a flag indicating whether grid values are defined per node (1) or per facet (0).
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @return 1 if the edge is valid, 0 if it is not.
     */
    virtual int isFacetEdgeValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed);

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
    static void fillColors(int id, float* buffer, int bufferLength, int elementsSize);

    /**
     * Fills the given buffer with indices data of the given object.
     * @param[in] the given object id.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    static int fillIndices(int id, int* buffer, int bufferLength, int logMask);
};

#endif
