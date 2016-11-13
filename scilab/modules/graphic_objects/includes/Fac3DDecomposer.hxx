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

#ifndef FAC3DDECOMPOSER_DECOMPOSER_HXX
#define FAC3DDECOMPOSER_DECOMPOSER_HXX

#include <string>

extern "C" {
#include <stdlib.h>
}

/**
 * Fac3D decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed Fac3D object's properties.
 * Fac3D data is represented by an N-gon set, as referred by the
 * various comments.
 */

class Fac3DDecomposer
{

private :

    /**
     * Specifies how the minimum and maximum values are computed from an input array
     * of per-vertex or per-face values.
     * The first enum value corresponds to either per-face or per-vertex input values
     * whereas the last two are only relevant to per-vertex input values.
     */
    enum MinMaxComputation
    {
        ALL_VALUES,   /** all input values are used */
        FACE_AVERAGE,   /** per-face averaged values */
        FIRST_VERTEX_VALUE   /** only each face's first value */
    };

    /**
     * Fills a buffer with texture coordinate computed from z values.
     * Colors are looked up in a color map, which is linearly mapped to the N-gon set's normalized z values.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[in] the z coordinate array.
     * @param[in] the number of N-gons.
     * @param[in] the number of vertices per N-gon.
     */
    static void fillNormalizedZColorsTextureCoordinates(float* buffer, int bufferLength, double* colormap, int colormapSize,
            double* z, int numGons, int numVerticesPerGon);

    /**
     * Fills a buffer with a single color.
     * The color buffer is filled with a single color value, looked up in a color map.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[in] the single color value used to fill the buffer.
     * @param[in] the number of N-gons.
     * @param[in] the number of vertices per N-gon.
     */
    static void fillConstantColorsTextureCoordinates(float* buffer, int bufferLength, double* colormap, int colormapSize,
            double colorValue, int numGons, int numVerticesPerGon);

    /**
     * Fills a buffer with color data.
     * Colors are looked up in a color map, using the N-gon set's color data array as an input.
     * @param[out] the buffer to fill.
     * @param[in] the buffer length in number of elements.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[in] the N-gon set's color values array.
     * @param[in] the color flag value.
     * @param[in] a flag specifying whether color values are defined per-vertex (1) or per-facet (0).
     * @param[in] a flag specifying whether color values are linearly mapped to the color map (0) or a direct mapping is used (1).
     * @param[in] the number of N-gons.
     * @param[in] the number of vertices per N-gon.
     */
    static void fillDataColorsTextureCoordinates(float* buffer, int bufferLength, double* colormap, int colormapSize,
            double* colors, int colorFlag, int perVertex, int dataMapping, int numGons, int numVerticesPerGon);

    /**
     * Computes the average value of an N-gon's vertex values.
     * @param[in] a pointer to the N-gon's first vertex value.
     * @param[in] the number of vertices.
     * @return the average value.
     */
    static double computeAverageValue(double* values, int numVertices);

    /**
     * Computes and returns an N-gon set's minimum and maximum values.
     * @param[in] the N-gon set's value array.
     * @param[in] the number of values
     * @param[in] the number of N-gons.
     * @param[in] the number of vertices per N-gon.
     * @param[in] specifies the way the minimum and maximum are computed: using all the input values, per-face averaged ones, or only each face's first value.
     *     The first case corresponds to either per-face or per-vertex values whereas the last two are valid only for per-vertex values.
     * @param[out] a pointer to the returned minimum value.
     * @param[out] a pointer to the returned maximum value.
     */
    static void computeMinMaxValues(double* values, int numValues, int numGons, int numVerticesPerGon, int minMaxComputation, double* valueMin, double* valueMax);

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
     * drawn according to the Fac3d object's properties and therefore does not
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
