/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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

#ifndef MESH_DATA_HXX
#define MESH_DATA_HXX

#include "Data3D.hxx"

extern "C" {
#include "BOOL.h"
}

/**
 * Mesh data class
 */

class MeshData: public Data3D
{

protected:

    /**
     * Vertex coordinates array
     * Contiguous (x, y, z) triplets
     */
    double* vertices;

    /** Element indices array
     * Contiguous (v0, v1, v2, ...) triplets
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

    /** Number of elements */
    unsigned int numberElements;

    unsigned int numberVerticesByElem;

public:
    /**
     * Constructor
     */
    MeshData(void);

    /**
     * Constructor
     * To be implemented
     */
    MeshData(unsigned int numberVertices, unsigned int numberElements, unsigned int numberVerticesByElem = 3);

    /**
     * Destructor
     */
    virtual ~MeshData();

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
     * Returns the number of index triplets (number of elements)
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
     * Returns the array of indices
     * @return a pointer to the array indices
     */
    unsigned int* getIndices(void);

    /**
     * Sets the number of index triplets
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
     * Converts a vertex index as seen by Scilab to an internal format vertex index
     * @param[in] scilabIndex the Scilab index to convert
     * @return the internal format vertex index
     */
    static unsigned int scilabIndexToIndex(unsigned int scilabIndex);
};

#endif
