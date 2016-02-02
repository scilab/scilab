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

#ifndef NGONGRIDDATA_DECOMPOSER_HXX
#define NGONGRIDDATA_DECOMPOSER_HXX

#include <string>

extern "C" {
#include <stdlib.h>
}

/**
 * NgonGridData decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed NgonGridData object's properties.
 *
 * To do: being able to specify either per-facet or per-vertex colors at execution time.
 *        Specifying per-facet colors and using flat shading at render time would reduce
 *        the vertex and color data duplication made necessary by using smooth shading
 *        when rendering flag-shaded facets (see the vertex and color data fill functions),
 *        with only a few additional modifications to the aforementioned functions.
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
    virtual void fillGridVertices(float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask,
                                  double* x, double* y, double* z, int numX, int numY);

    /**
     * Fills a buffer with color data from a grid.
     * Colors are looked up in a color map, which is linearly mapped to the grid's normalized z values.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of components taken by a color element (3 or 4).
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     */
    void fillNormalizedZGridColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
                                   double* z, int numX, int numY);

    /**
     * Fills a buffer with color data from a grid.
     * Colors are looked up in a color map, which is directly mapped to the grid's z values.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of components taken by a color element (3 or 4).
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     */
    void fillDirectGridColors(float* buffer, int bufferLength, int elementsSize, double* colormap, int colormapSize,
                              double* z, int numX, int numY);

    /**
     * Returns the coordinates of the (i,j) facet's four vertices, where (i,j) is the facet's
     * lower-left corner. Vertices are output in the following order: (i,j), (i+1,j), (i+1,j+1) and (i,j+1).
     * @param[in] the grid x-coordinate array.
     * @param[in] the grid y-coordinate array.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[out] the facet's vertices (4 (x,y,z) triplets).
     */
    void getFacetCoordinates(double* x, double* y, double* z, int numX, int numY, int i, int j,
                             double vertices[4][3]);

    /**
     * Returns the z-coordinate of the (i,j) grid point.
     * It treats z as a 1-element array, hence all grid points have the same z-value.
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
     * It treats z as a 1-element array, hence all grid points have the same z-value.
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
     * @param[in] the grid value array.
     * @param[in] the grid's number of points along the x-axis.
     * @param[in] the grid's number of points along the y-axis.
     * @param[in] the point's x index.
     * @param[in] the point's y index.
     * @return the (i,j) grid point's value.
     */
    virtual double getValue(double* values, int numX, int numY, int i, int j);

    /**
     * Fills a buffer with triangle indices from a decomposed grid.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the grid x-coordinate array.
     * @param[in] the grid y-coordinate array.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] a flag indicating whether grid values are defined per node (1) or per facet (0).
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @return the number of indices actually written.
     */
    int fillTriangleIndices(int* buffer, int bufferLength, int logMask, double* x, double* y, double* z, double* values, int perNodeValues,
                            int numX, int numY);

    /**
     * Decomposes facet (i,j) into triangles and outputs the resulting vertex indices, where (i,j) is
     * the facet's lower-left corner. As N-gon grid objects are plane, it always decomposes facets
     * exactly the same way. The output triangles' vertex indices are in counter-clockwise order.
     * @param[in] the grid x-coordinate array.
     * @param[in] the grid y-coordinate array.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] the facet vertices' indices (4-element array: (i,j), (i+1,j), (i+1,j+1) and (i,j+1) indices).
     * @param[out] the triangles' indices (6-element array: two consecutive triplets).
     */
    virtual void getFacetTriangles(double* x, double* y, double* z, int numX, int numY, int i, int j,
                                   int* facetVertexIndices, int* triangleVertexIndices);

    /**
     * Determines whether a facet is valid.
     * The facet is identified by its lower left-corner (i,j). It requires a flag (computed beforehand)
     * as an input which indicates whether the (i,j) to (i,j+1) edge is valid or not, and outputs
     * another flag indicating whether the (i+1,j) to (i+1,j+1) edge is valid or not.
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
     * upper-left corner (i,j+1). The edge's validity depends only
     * on its endpoints' z coordinates.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid value array.
     * @param[in] a flag indicating whether grid values are defined per node (1) or per facet (0).
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the lower-left corner's x index.
     * @param[in] the lower-left corner's y index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @return 1 if the edge valid, 0 if it is not.
     */
    virtual int isFacetEdgeValid(double* z, double* values, int perNodeValues, int numX, int numY, int i, int j, int logUsed);

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

    /**
     * Computes a facet's average z value.
     * @param[in] the grid z-coordinate array.
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the facet's lower-left corner x index.
     * @param[in] the facet's lower-left corner y index.
     * @return the facet's average z value.
     */
    static double computeFacetAverageZValue(double* z, int numX, int numY, int i, int j);

    /**
     * Writes the color value of a facet's four vertices into a buffer.
     * The color is constant across the whole facet, hence four identical
     * color values are consecutively written.
     * @param[out] the buffer which is written to.
     * @param[in] the buffer offset of the first vertex's color.
     * @param[in] the facet color (3 or 4-element array).
     * @param[in] the number of components taken by a color element (3 or 4).
     */
    static void writeFacetColorToBuffer(float* buffer, int bufferOffset, float* color, int elementsSize, bool hasTransparency = false);

    /**
     * Returns the index of a facet's first vertex (its lower-left corner).
     * This index corresponds to the location of the facet's first vertex in an array whose elements
     * are sets of four vertices, a set corresponding to a single grid facet, with shared vertices
     * being duplicated. For facet (i,j), vertices are ordered as follows: (i,j), (i+1,j), (i,j+1), (i+1,j+1) .
     * @param[in] the grid's number of vertices along the x-axis.
     * @param[in] the grid's number of vertices along the y-axis.
     * @param[in] the facet's lower-left corner x index.
     * @param[in] the facet's lower-left corner y index.
     * @return the index of the facet's first vertex.
     */
    static int getFirstVertexIndex(int numX, int numY, int i, int j);

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
    static int getDataSize(int id);

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
     * Returns the number of indices for the given object.
     * @param[in] the given object id.
     * @return the object's number of indices.
     */
    static int getIndicesSize(int id);

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

/**
 * Specifies whether per-vertex or per-facet colors are used (0 corresponds to
 * per-facet color values, any other value to per-vertex colors) for all the
 * Ngon grid-derived objects, by commenting out the related code blocks.
 * Temporary since this should be specified as a parameter of the vertex,
 * index and color fill functions, independently for each Ngon grid-derived
 * object type.
 */
#define PER_VERTEX_VALUES    0

/**
 * The default z-coordinate value for plane grid objects
 * (Grayplot and Matplot) when the logarithmic scale is used.
 * It is added to their z-shift value to obtain the actual z-coordinate.
 */
#define DEFAULT_LOG_COORD_Z 1.0

#endif
