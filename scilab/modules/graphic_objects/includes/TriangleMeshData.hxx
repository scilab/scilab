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

#ifndef TRIANGLE_MESH_DATA_HXX
#define TRIANGLE_MESH_DATA_HXX

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
    /**
     * Constructor
     */
    TriangleMeshData(void);

    /**
     * Constructor
     * To be implemented
     */
    TriangleMeshData(unsigned int numberVertices, unsigned int numberTriangles);

    /**
     * Destructor
     */
    virtual ~TriangleMeshData();

    /**
     * Returns the identifier associated to a property name
     * @param[in] propertyName the property name
     * @return the property identifier
     */
    int getPropertyFromName(int propertyName);

    /**
     * Sets a data property
     * @param[in] property the property identifier
     * @param[in] value a pointer to the property values
     * @param[in] numElements the number of elements to set
     * @return 1 if the property has been successfully set, 0 otherwise
     */
    int setDataProperty(int property, void const* value, int numElements);

    /**
     * Returns a data property
     * @param[in] property the property identifier
     * @param[out] a pointer to a pointer to the returned property values
     */
    void getDataProperty(int property, void **_pvData);

    /**
     * Returns the number of vertices composing the mesh
     * @return the number of vertices
     */
    unsigned int getNumVertices();

    /**
     * Sets the number of vertices
     * Resizes the vertex array if required
     * @param[in] numVertices the number of vertices to set
     * @return 1 if the number of vertices has been successfully set, 0 otherwise (failed allocation)
     */
    int setNumVertices(unsigned int numVertices);

    /**
     * Returns the number of index triplets (number of triangles)
     * @return the number of index triplets
     */
    unsigned int getNumIndices();

    /**
     * Returns the array of vertex coordinates
     * @return a pointer to the array of vertex coordinates
     */
    double* getVertices(void);

    /**
     * Sets vertex coordinates
     * @param[in] vertices a pointer the array of vertex {x,y,z} coordinates to set
     * @param[in] numElements the number of vertices
     */
    void setVertices(double const* vertices, unsigned int numElements);

    /**
     * Returns the array of triangle indices
     * @return a pointer to the array of triangle indices
     */
    unsigned int* getIndices(void);

    /**
     * Sets the number of index triplets (number of triangles)
     * Resizes the array of indices if required
     * @param[in] numIndices the number of index triplets to set
     * @return 1 if the number of index triplets has been successfully set, 0 otherwise (failed allocation)
     */
    int setNumIndices(unsigned int numIndices);

    /**
     * Sets the array of index triplet values
     * @param[in] indices a pointer to the array of index triplet values
     * @param[in] numElements the number of triplets
     */
    void setIndices(unsigned int const* indices, unsigned int numElements);

    /**
     * Sets the x coordinates
     * @param[in] data a pointer to the array of x coordinates
     * @param[in] numElements the number of x coordinates to set
     */
    void setDataX(double const* data, unsigned int numElements);

    /**
     * Sets the y coordinates
     * @param[in] data a pointer to the array of y coordinates
     * @param[in] numElements the number of y coordinates to set
     */
    void setDataY(double const* data, unsigned int numElements);

    /**
     * Sets the z coordinates
     * @param[in] data a pointer to the array of z coordinates
     * @param[in] numElements the number of z coordinates to set
     */
    void setDataZ(double const* data, unsigned int numElements);

    /**
     * Returns the array of per-vertex values
     * @return a pointer to the array of per-vertex values
     */
    double* getValues(void);

    /**
     * Sets the array of per-vertex values
     * @param[in] data a pointer to the array of per-vertex values
     * @param[in] numElements the number of values to set
     */
    void setValues(double const* data, unsigned int numElements);

    /**
     * Resets the vertex coordinates
     */
    void resetCoordinates(void);

    /**
     * Converts a triangle vertex index as seen by Scilab to an internal format triangle vertex index
     * @param[in] scilabIndex the Scilab index to convert
     * @return the internal format triangle vertex index
     */
    static unsigned int scilabIndexToIndex(unsigned int scilabIndex);
};

#endif
