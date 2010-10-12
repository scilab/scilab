/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef TRIANGLE_MESH_DATA_H
#define TRIANGLE_MESH_DATA_H

#include "Data3D.hxx"

extern "C" {
#include "BOOL.h"
}

/**
 * Triangle mesh data class
 */

class TriangleMeshData: public Data3D
{

protected:

    /**
     * Vertex coordinates array
     * Contiguous (x, y, z) triplets
     */
    double* vertices;

    /** Triangle indices array
     * Contiguous (v0, v1, v2) triplets
     */
    unsigned int* indices;

    /**
     * Per-vertex or per-facet scalar values
     * Considered to be per-vertex for now
     * To be correctly implemented
     */
    double* values;

    /** Number of vertices */
    unsigned int numberVertices;

    /** Number of triangles */
    unsigned int numberTriangles;

public:
    TriangleMeshData(void);

    /* To be implemented */
    TriangleMeshData(unsigned int numberVertices, unsigned int numberTriangles);

    virtual ~TriangleMeshData();

    /**
     * Returns the property value (identifier) associated to a name
     * @return the property identifier
     */
    int getPropertyFromName(char* propertyName);

    /**
     * Sets a data property
     * @param property the property identifier
     * @param value the property values
     * @param numElements the number of elements to set
     * @return 1 if the property has correctly been set, 0 otherwise
     */
    int setDataProperty(int property, void* value, int numElements);

    /**
     * Returns a data property
     * @param property the property identifier
     * @return a pointer to the data property
     */
    void* getDataProperty(int property);

    /**
     * Returns the number of vertices composing the mesh
     * @return the number of vertices
     */
    unsigned int getNumVertices();

    /**
     * Sets the number of vertices
     * Resizes the vertex array if required
     * @param numVertices the number of vertices to set
     * @return 1 if the number of vertices has been correctly set, 0 otherwise (failed allocation)
     */
    int setNumVertices(unsigned int numVertices);

    /**
     * Returns the number of index triplets (number of triangles)
     * @return the number of index triplets
     */
    unsigned int getNumIndices();

    /**
     * Returns the array of vertex coordinates
     * @return the array of vertex coordinates
     */
    double* getVertices(void);

    /**
     * Sets vertex values
     * @param vertices the array of vertex {x,y,z} coordinates to set
     * @param numElements the number of vertices
     */
    void setVertices(double* vertices, unsigned int numElements);

    /**
     * Returns the array of triangle indices
     * @return the array of triangle indices
     */
    unsigned int* getIndices(void);

    /**
     * Sets the number of index triplets (number of triangles)
     * Resizes the array of indices if required
     * @param numIndices the number of index triplets to set
     * @return 1 if the number of index triplets has been correctly set, 0 otherwise (failed allocation)
     */
    int setNumIndices(unsigned int numIndices);

    /**
     * Sets the array of index triplet values
     * @param indices the array of index triplet values
     * @param numElements the number of triplets
     */
    void setIndices(unsigned int* indices, unsigned int numElements);

    /**
     * Sets the x coordinates
     * @param data the array of x coordinates
     * @param numElements the number of x coordinates to set
     */
    void setDataX(double* data, unsigned int numElements);

    /**
     * Sets the y coordinates
     * @param data the array of y coordinates
     * @param numElements the number of y coordinates to set
     */
    void setDataY(double* data, unsigned int numElements);

    /**
     * Sets the z coordinates
     * @param data the array of z coordinates
     * @param numElements the number of z coordinates to set
     */
    void setDataZ(double* data, unsigned int numElements);

    /**
     * Returns the array of per-vertex values
     * @return the array of per-vertex values
     */
    double* getValues(void);

    /**
     * Sets the array of per-vertex values
     * @param data the array of per-vertex values
     * @param numElements the number of values to set
     */
    void setValues(double* data, unsigned int numElements);

    /**
     * Resets the vertex coordinates
     */
    void resetCoordinates(void);

    /**
     * Converts a triangle vertex index as seen by Scilab to an internal format triangle vertex index
     * @param scilabIndex the Scilab index to convert
     * @returns the internal format triangle vertex index
     */
    static unsigned int scilabIndexToIndex(unsigned int scilabIndex);
};

#endif
