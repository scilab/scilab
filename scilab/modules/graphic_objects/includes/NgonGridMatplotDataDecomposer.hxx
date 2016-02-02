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

#ifndef NGONGRIDMATPLOTDATA_DECOMPOSER_HXX
#define NGONGRIDMATPLOTDATA_DECOMPOSER_HXX

#include <string>
#include "ColorComputer.hxx"

extern "C"
{
#include "Matplot.h"
}

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
     * Fills a buffer with vertex data from a grid.
     * In the case of Matplot objects, vertex coordinates are computed using the coordinates of the grid's lower-left corner
     * and the distances between two adjacent vertices for both the x and y axes, which we label respectively (xmin, ymin) and
     * (xd, yd). The relevant values are used as the x and y coordinate arrays (see the parameter descriptions).
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation factor to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the grid x-coordinate array (2 elements: xmin, xd).
     * @param[in] the grid y-coordinate array (2 elements: ymin, yd).
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     */
    virtual void fillGridVertices(float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask,
                                  double* x, double* y, double* z, int numX, int numY);

    /**
     * Determines whether a facet is valid.
     * The facet is identified by its lower left-corner (i,j).
     * For a Matplot object, facet (i,j)'s validity depends only on its z coordinate and its (i,j) value,
     * edge validity being therefore irrelevant. The flag specifying per-node values should be 0,
     * as Matplot grid values are defined per facet, although it is actually unused.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] a flag indicating whether grid values are defined per node (1) or per facet (0). Unused.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used. Unused.
     * @param[in] (i,j) to (i,j+1) edge validity flag. Unused.
     * @param[out] a pointer to the output (i+1,j) to (i+1,j+1) edge validity flag. Always set to 1.
     * @return 1 if the facet is valid, 0 if it is not.
     */
    virtual int isFacetValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed, int currentEdgeValid, int* nextEdgeValid);

    /**
     * Determines whether the left edge of a facet is valid.
     * For a Matplot object, edge validity is irrelevant, it is therefore always equal to 1.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] a flag indicating whether grid values are defined per node (1) or per facet (0).
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @return Always 1.
     */
    virtual int isFacetEdgeValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed);

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
     * Fills the given buffer with indices data of the given object.
     * @param[in] the given object id.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @return the number of indices actually written.
     */
    static int fillIndices(int id, int* buffer, int bufferLength, int logMask);

    template <typename T>
    inline static void fillColorsByIndex(T * indices, float * buffer, int elementsSize, const int nbRow, const int nbCol, double * colormap, const int colormapSize)
    {
        float facetColor[3];
        T index;
        int bufferOffset = 0;

        for (int j = 0; j < nbRow; j++)
        {
            for (int i = 0; i < nbCol; i++)
            {
                index = indices[nbRow - 1 + i * nbRow - j];
                ColorComputer::getDirectColor(index - 1, colormap, colormapSize, facetColor);
                writeFacetColorToBuffer(buffer, bufferOffset, facetColor, elementsSize);
                bufferOffset += 4 * elementsSize;
            }
        }
    }

    static void getRGBAData(ImageType imagetype, DataType datatype, GLType gltype, void * data, float * buffer, int elementsSize, const int nbRow, const int nbCol, double * colormap, const int colormapSize);

};

#endif
