/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Pedro SOUZA
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

#include <math.h>
#include "NormalGenerator.hxx"

#define PLUS3(a, b, c) (c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];
#define MINUS3(a, b, c) (c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];
#define CROSS3(a, b, c) (c)[0]=(a)[1]*(b)[2] - (a)[2]*b[1];(c)[1]=(a)[2]*(b)[0] - (a)[0]*(b)[2];(c)[2]=(a)[0]*(b)[1] - (a)[1]*(b)[0];
#define COPY3(a, b) (a)[0]=(b)[0];(a)[1]=(b)[1];(a)[2]=(b)[2];

inline void normalize3(float* v)
{
    float inv_norm = 1.0f / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] *= inv_norm;
    v[1] *= inv_norm;
    v[2] *= inv_norm;
}

bool CalculateGridNormalFlat(float* position, float* buffer, int bufferLength, int elementSize)
{
    if (elementSize < 3)
    {
        return false;
    }
    for (int i = 0; i < bufferLength; i += 4 * elementSize)
    {
        float *p1 = &position[i],
               *p2 = &position[i + elementSize],
                *p3 = &position[i + 2 * elementSize],
                 *p4 = &position[i + 3 * elementSize];

        float v1[3], v2[3], n1[3], n2[3], n1pn2[3];
        //v1 = p2-p1
        MINUS3(p2, p1, v1);
        MINUS3(p4, p1, v2);

        //n1 = v1 x v2
        CROSS3(v1, v2, n1);

        MINUS3(p4, p1, v1);
        MINUS3(p3, p1, v2);

        CROSS3(v1, v2, n2);

        normalize3(n1);
        normalize3(n2);
        PLUS3(n1, n2, n1pn2);

        normalize3(n1pn2);


        COPY3(&buffer[i], n1pn2);
        COPY3(&buffer[i + elementSize], n1pn2);
        COPY3(&buffer[i + 2 * elementSize], n1pn2);
        COPY3(&buffer[i + 3 * elementSize], n1pn2);
    }
    return true;
}

bool CalculateGridNormalSmooth(float* position, float* buffer, int bufferLength, int elementSize, int numX, int numY)
{
    if (elementSize < 3)
    {
        return false;
    }
    for (int i = 0; i < bufferLength; i += 4 * elementSize)
    {
        float *p1 = &position[i],
               *p2 = &position[i + elementSize],
                *p3 = &position[i + 2 * elementSize],
                 *p4 = &position[i + 3 * elementSize];

        float v1[3], v2[3], n1[3], n2[3], n1pn2[3];
        //v1 = p2-p1
        MINUS3(p2, p1, v1);
        MINUS3(p4, p1, v2);

        //n1 = v1 x v2
        CROSS3(v1, v2, n1);

        MINUS3(p4, p1, v1);
        MINUS3(p3, p1, v2);

        CROSS3(v1, v2, n2);

        normalize3(n1);
        normalize3(n2);
        PLUS3(n1, n2, n1pn2);

        normalize3(n1pn2);


        COPY3(&buffer[i], n1pn2);
        COPY3(&buffer[i + elementSize], n1);
        COPY3(&buffer[i + 2 * elementSize], n2);
        COPY3(&buffer[i + 3 * elementSize], n1pn2);
    }

    // average normals in x axis
    for (int i = 0; i < numX - 1; ++i)
    {
        for (int j = 0; j < numY - 2; ++j)
        {
            float sum[3];
            float *p2, *p3, *pj0, *pj1;
            p2 = getGridNormal(buffer, numX, numY, elementSize, i, j, 2);
            p3 = getGridNormal(buffer, numX, numY, elementSize, i, j, 3);

            pj0 = getGridNormal(buffer, numX, numY, elementSize, i, j + 1, 0);
            pj1 = getGridNormal(buffer, numX, numY, elementSize, i, j + 1, 1);

            PLUS3(pj0, p2, sum);
            COPY3(p2, sum);
            COPY3(pj0, sum);

            PLUS3(pj1, p3, sum);
            COPY3(p3, sum);
            COPY3(pj1, sum);
        }
    }

    // average normals in y axis
    for (int j = 0; j < numY - 1; ++j)
    {
        for (int i = 0; i < numX - 2; ++i)
        {
            float sum[3];
            float *p1, *p3, *pi0, *pi2;
            p1 = getGridNormal(buffer, numX, numY, elementSize, i, j, 1);
            p3 = getGridNormal(buffer, numX, numY, elementSize, i, j, 3);

            pi0 = getGridNormal(buffer, numX, numY, elementSize, i + 1, j, 0);
            pi2 = getGridNormal(buffer, numX, numY, elementSize, i + 1, j, 2);

            PLUS3(pi0, p1, sum);
            COPY3(p1, sum);
            COPY3(pi0, sum);

            PLUS3(pi2, p3, sum);
            COPY3(p3, sum);
            COPY3(pi2, sum);
        }
    }
    return true;
}

float * getGridNormal(float *buffer, int numX, int numY, int elementSize, int i, int j, int k)
{
    //int idx = (numX-1)*j*4*elementSize + i*4*elementSize + k*elementSize;
    int idx = elementSize * (k + 4 * (j * (numX - 1) + i));
    return &buffer[idx];
}

bool CalculatePolygonNormalFlat(float* position, float* buffer, int bufferLength, int elementSize, int polygonVertices)
{
    for (int i = 0; i < bufferLength; i += (elementSize * polygonVertices))
    {
        float v1[3], v2[3], n[3], sum[] = {0.0f, 0.0f, 0.0f};

        for (int j = 0; j < polygonVertices - 2; ++j)
        {
            float *p1 = &position[i],
                   *p2 = &position[i + (j + 2) * elementSize],
                    *p3 = &position[i + (j + 1) * elementSize];

            MINUS3(p2, p1, v1);
            MINUS3(p3, p1, v2);
            CROSS3(v1, v2, n);
            normalize3(n);

            PLUS3(n, sum, sum);
        }
        for (int j = 0; j < polygonVertices; ++j)
        {
            COPY3(&buffer[i + j * elementSize], sum);
        }
    }
    return true;
}

