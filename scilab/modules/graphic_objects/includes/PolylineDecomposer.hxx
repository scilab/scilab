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

#ifndef POLYLINE_DECOMPOSER_HXX
#define POLYLINE_DECOMPOSER_HXX

#include <string>

extern "C" {
#include <stdio.h>
}

/**
 * Polyline decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed Polyline's properties.
 * It only has static member functions since it stores no actual state.
 *
 * To be optimized: there is a lot of redundant code shared by the
 * different fillIndices methods. The decomposition loop code is present
 * in each method, whereas the only type-specific operations are those
 * actually filling the index buffer.
 * Furthermore, the loop is executed twice for methods outputting both
 * line segments plus type-specific segments (e.g vertical lines). This
 * can be solved by interleaving line and type-specifing segment indices.
 */

class PolylineDecomposer
{

private :

    /**
     * Fills a buffer with vertex data from a polyline decomposed into consecutive segments.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation value to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillSegmentsDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Writes the coordinates of a polyline vertex to a buffer.
     * Each coordinate may correspond to a different vertex (as specified by an index).
     * @param[out] the buffer.
     * @param[in] the buffer offset of the vertex's first component.
     * @param[in] the polyline coordinate array.
     * @param[in] the indices of the polyline vertex(ices) to fetch (3 elements, 1 index for each of the X,Y and Z components).
     * @param[in] the polyline's number of points.
     * @param[in] the number of components per buffer element.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation value to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     */
    static void getAndWriteVertexToBuffer(float* buffer, int offset, double* coordinates, int* vertexIndices, int nPoints, int elementsSize,
                                          double* xshift, double* yshift, double* zshift, int coordinateMask, double* scale, double* translation, int logMask);

    /**
     * Fills a buffer with vertex data from a polyline using a staircase decomposition.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation value to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillStairDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Fills a buffer with vertex data from a polyline decomposed into a series of vertical lines.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation value to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillVerticalLinesDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Writes one component of the coordinates of a bar's four vertices into a buffer.
     * @param[out] the buffer which is written to.
     * @param[in] the offsets of the four vertices into the buffer (4-element array).
     * @param[in] the offset specifying which component (0, 1 or 2 for respectively X, Y or Z) is written.
     * @param[in] the coordinates of the bar's vertices (4-element array, 1 component per vertex).
     * @param[in] the shift value applied to the 4 coordinates.
     * @param[in] a flag specifying whether the shift is applied or not.
     * @param[in] the conversion scale factor to apply.
     * @param[in] the conversion translation factor to apply.
     * @param[in] a flag specifying whether logarithmic coordinates are used or not.
     */
    static void writeBarVerticesToBuffer(float* buffer, int* offsets, int componentOffset, double* coordinates, double shift, int shiftUsed, double scale, double translation, int logUsed);

    /**
     * Fills a buffer with vertex data from a polyline decomposed into a series of vertical bars and consecutive segments.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation factor to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillVerticalBarsDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Fills a buffer with vertex data from a polyline decomposed into a series of horizontal bars and consecutive segments.
     * x, y coordinates and shift coordinates are swapped relative to the decomposition into vertical bars.
     * To do: refactor with fillVerticalBarsDecompositionVertices as they are very similar.
     * @param[in] the id of the given polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of coordinates taken by one element in the buffer.
     * @param[in] the byte mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
     * @param[in] the conversion scale factor to apply to data.
     * @param[in] the conversion translation factor to apply to data.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     */
    static void fillHorizontalBarsDecompositionVertices(int id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Returns the number of triangle indices of a polyline decomposed into consecutive segments with arrow heads.
     * The triangles correspond to the arrow heads.
     * @param[in] the polyline's number of points.
     * @param[in] a flag indicating whether the polyline is closed or not.
     * @return the number of triangle indices.
     */
    static int getArrowTriangleIndicesSize(int nPoints, int closed);

    /**
     * Returns the number of triangle indices of a polyline decomposed into a series of bars and consecutive segments.
     * @param[in] the polyline's number of points.
     * @return the number of triangle indices.
     */
    static int getBarsDecompositionTriangleIndicesSize(int nPoints);

    /**
     * Fills a buffer with triangles indices obtained by triangulation of a polygon whose contour
     * is defined by the polyline.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @param[in] the fill mode flag.
     * @param[in] the polyline style.
     * @return the number of indices actually written.
     */
    static int fillTriangleIndices(int id, int* buffer, int bufferLength,
                                   int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int fillMode, int polylineStyle);

    /**
     * Fills a buffer with the triangle indices of the arrow heads of a polyline's segments.
     * One triangle is output for each valid segment.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @return the number of indices actually written.
     */
    static int fillArrowTriangleIndices(int id, int* buffer, int bufferLength,
                                        int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Fills a buffer with the triangle indices of a polyline decomposed into a series of bars and consecutive segments.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @return the number of indices actually written.
     */
    static int fillBarsDecompositionTriangleIndices(int id, int* buffer, int bufferLength,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift);

    /**
     * Returns the number of segment indices of a polyline decomposed into consecutive segments.
     * @param[in] the polyline's number of points.
     * @param[in] the line mode flag.
     * @param[in] a flag indicating whether the polyline is closed or not.
     * @return the number of segment indices.
     */
    static int getSegmentsDecompositionSegmentIndicesSize(int nPoints, int lineMode, int closed);

    /**
     * Returns the number of segment indices of a polyline decomposed into a series of steps.
     * @param[in] the polyline's number of points.
     * @param[in] the line mode flag.
     * @param[in] a flag indicating whether the polyline is closed or not.
     * @return the number of segment indices.
     */
    static int getStairDecompositionSegmentIndicesSize(int nPoints, int lineMode, int closed);

    /**
     * Returns the number of segment indices of a polyline decomposed into a series of vertical lines and consecutive segments.
     * @param[in] the polyline's number of points.
     * @param[in] the line mode flag.
     * @return the number of segment indices.
     */
    static int getVerticalLinesDecompositionSegmentIndicesSize(int nPoints, int lineMode);

    /**
     * Returns the number of segment indices of a polyline decomposed into a series of bars and consecutive segments.
     * @param[in] the polyline's number of points.
     * @param[in] the line mode flag.
     * @return the number of segment indices.
     */
    static int getBarsDecompositionSegmentIndicesSize(int nPoints, int lineMode);

    /**
     * Fills a buffer with the segment indices of a polyline decomposed into consecutive segments.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @param[in] the line mode flag.
     * @param[in] a flag indicating whether the polyline is closed or not.
     * @return the number of indices actually written.
     */
    static int fillSegmentsDecompositionSegmentIndices(int id, int* buffer, int bufferLength,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode, int closed);

    /**
     * Fills a buffer with the segment indices of a polyline decomposed into a series of steps.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @param[in] the line mode flag.
     * @param[in] a flag indicating whether the polyline is closed or not.
     * @return the number of indices actually written.
     */
    static int fillStairDecompositionSegmentIndices(int id, int* buffer, int bufferLength,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode, int closed);

    /**
     * Fills a buffer with the segment indices of a polyline decomposed into a series of vertical lines and consecutive segments.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @param[in] the line mode flag.
     * @return the number of indices actually written.
     */
    static int fillVerticalLinesDecompositionSegmentIndices(int id, int* buffer, int bufferLength,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode);

    /**
     * Fills a buffer with the segment indices of a polyline decomposed into a series of bars and consecutive segments.
     * @param[in] the id of the polyline.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the bit mask specifying whether logarithmic coordinates are used.
     * @param[in] the polyline coordinate array.
     * @param[in] the polyline's number of points.
     * @param[in] the polyline x-shift array.
     * @param[in] the polyline y-shift array.
     * @param[in] the polyline z-shift array.
     * @param[in] the line mode flag.
     * @return the number of indices actually written.
     */
    static int fillBarsDecompositionSegmentIndices(int id, int* buffer, int bufferLength,
            int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode);

    /**
     * Returns the coordinates of a polyline point, modified by their corresponding shift values.
     * @param[in] the coordinate array
     * @param[in] the x-shift array.
     * @param[in] the y-shift array.
     * @param[in] the z-shift array.
     * @param[in] the polyline's number of points.
     * @param[in] the point index.
     * @param[out] the returned x coordinate.
     * @param[out] the returned y coordinate.
     * @param[out] the returned z coordinate.
     */
    static void getShiftedPolylinePoint(double* coordinates, double* xshift, double* yshift, double* zshift, int nPoints, int index,
                                        double* x, double* y, double* z);


public :

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
     * Fills the given buffer with color data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] the number of components taken by one element in the buffer (3 or 4).
     */
    static void fillColors(int id, float* buffer, int bufferLength, int elementsSize);

    /**
     * Fills the given buffer with texture coordinate data from the given object.
     * @param[in] the id of the given object.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     */
    static void fillTextureCoordinates(int id, float* buffer, int bufferLength);

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

    /**
     * Returns the number of wireframe indices for the given object.
     * It is equal to twice the number of segments that can be at most
     * drawn according to the polyline's properties and therefore does not
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
