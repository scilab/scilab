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

#ifndef MESHFECDATA_DECOMPOSER_HXX
#define MESHFECDATA_DECOMPOSER_HXX

/**
 * TriangleMeshFecData decomposer class
 * Determines the vertices and the segments indices to be rendered
 * as a function of the decomposed TriangleMeshFecData object's properties.
 * It only has static member functions since it stores no actual state.
 */

class MeshFecDataDecomposer
{

private :

    /**
     * Computes and returns the minimum and maximum values of an array.
     * @param[in] the value array.
     * @param[in] the array's number of values.
     * @param[out] a pointer to the returned minimum value.
     * @param[out] a pointer to the returned maximum value.
     */
    static void computeMinMaxValues(double* values, int numValues, double* valueMin, double* valueMax);

    /**
     * Determines whether a face's vertices are valid or not.
     * @param[in] the coordinates array.
     * @param[in] the first vertex index.
     * @param[in] the second vertex index.
     * @param[in] the third vertex index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @return 1 if the face is valid, 0 if it is not.
     */
    static int areFaceVerticesValid(double* coordinates, int v0, int v1, int v2, int logMask);

    /**
     * Determines whether a face's vertices are valid or not.
     * @param[in] the coordinates array.
     * @param[in] the first vertex index.
     * @param[in] the second vertex index.
     * @param[in] a flag specifying whether logarithmic coordinates are used.
     * @return 1 if the face is valid, 0 if it is not.
     */
    static int areSegmentVerticesValid(double* coordinates, int v0, int v1, int logMask);

    /**
     * Determines whether a face's vertex values are valid or not.
     * @param[in] the values array.
     * @param[in] the first vertex index.
     * @param[in] the second vertex index.
     * @param[in] the third vertex index.
     * @return 1 if all face values are valid, 0 if not.
     */
    static int areFaceValuesValid(double* values, int v0, int v1, int v2);

    /**
     * Determines whether a face's vertex values are valid or not.
     * @param[in] the values array.
     * @param[in] the first vertex index.
     * @param[in] the second vertex index.
     * @return 1 if all face values are valid, 0 if not.
     */
    static int areSegmentValuesValid(double* values, int v0, int v1);

    /**
     * Determines whether all the vertex indices making up a face are valid.
     * To be valid, an index must belong to the interval [0, numVertices-1]
     * @param[in] the number of vertices giving the largest valid vertex index.
     * @param[in] the first vertex index.
     * @param[in] the second vertex index.
     * @param[in] the third vertex index.
     * @return 1 if all indices are valid, 0 if not.
     */
    static int areFaceIndicesValid(int numVertices, int v0, int v1, int v2);

    /**
     * Determines whether all the vertex indices making up a face are valid.
     * To be valid, an index must belong to the interval [0, numVertices-1]
     * @param[in] the number of vertices giving the largest valid vertex index.
     * @param[in] the first vertex index.
     * @param[in] the second vertex index.
     * @return 1 if all indices are valid, 0 if not.
     */
    static int areSegmentIndicesValid(int numVertices, int v0, int v1);

    /**
     * Returns the coordinates of a single vertex.
     * @param[in] the coordinates array.
     * @param[in] the vertex index.
     * @param[out] a pointer to the returned coordinates {x, y, z}.
     */
    static void getVertexCoordinates(double* coordinates, int index, double* vertexCoordinates);

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
     * Fills the given buffer with texture coordinates data from the given object.
     * @param[in] the id of the given object.
     * @param[in] the buffer to fill.
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
     * drawn according to the Fec object's properties and therefore does not
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
