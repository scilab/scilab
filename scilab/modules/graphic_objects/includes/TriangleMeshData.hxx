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

#include <string>

#include "Data3D.hxx"

extern "C" {
#include "BOOL.h"

#include <stdio.h>
}

/**
 * Triangle mesh data stub class
 * To be fully implemented
 */

class TriangleMeshData : public Data3D
{

private :

    /** Vertex coordinates array */
    double* vertices;

    /** Triangle indices array */
    unsigned int* indices;

    /** Number of vertices */
    unsigned int numberVertices;

    /** Number of triangles */
    unsigned int numberTriangles;

public :
    TriangleMeshData(void)
    {
        vertices = NULL;
        indices = NULL;

        numberVertices = 0;
        numberTriangles = 0;
    }

    /* To be implemented */
    TriangleMeshData(unsigned int numberVertices, unsigned int numberTriangles)
    {
	vertices = new double[3*numberVertices];

        indices = new unsigned int[3*numberTriangles];

        this->numberVertices = numberVertices;
        this->numberTriangles = numberTriangles;
    }

    double* getData(void)
    {
        return vertices;
    }

    unsigned int* getIndices(void)
    {
        return indices;
    }

    /* To be implemented */
    void setData(double* data)
    {

    }

    /* To be implemented */
    void setIndices(unsigned int* indices)
    {

    }

};

#endif
