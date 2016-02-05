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

#include "DecompositionUtils.hxx"

extern "C"
{
#include <math.h>
#include <float.h>
#ifdef _MSC_VER
#define isnan _isnan
    // isinf(x)
    // | +%inf -> 1
    // | -%inf -> -1
    // | _ -> 0
#define isinf(x) (_fpclass(x)==_FPCLASS_PINF?1:(_fpclass(x)==_FPCLASS_NINF?-1:0))
#endif
}

int DecompositionUtils::isANumber(double x)
{
    if (isnan(x))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int DecompositionUtils::isFinite(double x)
{
    if (isinf(x))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int DecompositionUtils::isValid(double x)
{
    if (isnan(x) || isinf(x))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int DecompositionUtils::isValid(double x, double y, double z)
{
    if (isnan(x) || isnan(y) || isnan(z) || isinf(x) || isinf(y) || isinf(z))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int DecompositionUtils::isValid(double x, double y)
{
    if (isnan(x) || isnan(y) || isinf(x) || isinf(y))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

double DecompositionUtils::getLog10Value(double value)
{
    return log10(value);
}

int DecompositionUtils::isLogValid(double x)
{
    if (x > 0.0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int DecompositionUtils::isLogValid(double x, double y, double z, int logMask)
{
    int valid = 1;

    if (logMask & 0x1)
    {
        valid &= (x > 0.0);
    }

    if (logMask & 0x2)
    {
        valid &= (y > 0.0);
    }

    if (logMask & 0x4)
    {
        valid &= (z > 0.0);
    }

    return valid;
}

int DecompositionUtils::isLogValid(double x, double y, int logMask)
{
    int valid = 1;

    if (logMask & 0x1)
    {
        valid &= (x > 0.0);
    }

    if (logMask & 0x2)
    {
        valid &= (y > 0.0);
    }

    return valid;
}

double DecompositionUtils::getMaxDoubleValue(void)
{
    return DBL_MAX;
}

double DecompositionUtils::getMinDoubleValue(void)
{
    return DBL_MIN;
}

double DecompositionUtils::getAbsoluteValue(double value)
{
    return fabs(value);
}

double DecompositionUtils::getSquareRoot(double value)
{
    return sqrt(value);
}

/*
 * Decomposes a rectangle into two adjacent triangles.
 * The rectangle's vertices are supposed to be specified in
 * counter-clockwise order, with 0 corresponding to the former's lower-left vertex.
 * The two output triangles' vertex indices are also specified in
 * counter-clockwise order.
 */
void DecompositionUtils::getDecomposedRectangleTriangleIndices(int* indices)
{
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 3;
}

/* To do: use a Vector3d class to perform vector operations. */
void DecompositionUtils::getDecomposedQuadTriangleIndices(double vertices[4][3], int* facetVertexIndices, int* triangleVertexIndices)
{
    /* The two decompositions' midpoints */
    double mid0[3];
    double mid1[3];

    /* The vectors from one midpoint to its opposite vertices */
    double mo0[3];
    double mo1[3];

    double nmo0 = 0.;
    double nmo1 = 0.;

    double dot0 = 0.;
    double dot1 = 0.;

    double denom = 0.;

    /*
     * The input vertices are given in counter-clockwise order, from v0 to v3.
     * Two decompositions are possible: either (v0,v1,v2) and (v0,v2,v3) or (v1,v2,v3) and (v1,v3,v0).
     * The best one is the one that yields the most coplanar triangles. To estimate this, for each configuration,
     * we compute the midpoint of the triangles' shared edge, which are respectively mid0 and mid1.
     * The angles (v1 mid0 v3) and (v2 mid1 v0) give an approximation of the angles between the two triangles' planes
     * for respectively the first and second condigurations.
     */

    mid0[0] = 0.5 * (vertices[0][0] + vertices[2][0]);
    mid0[1] = 0.5 * (vertices[0][1] + vertices[2][1]);
    mid0[2] = 0.5 * (vertices[0][2] + vertices[2][2]);
    mid1[0] = 0.5 * (vertices[1][0] + vertices[3][0]);
    mid1[1] = 0.5 * (vertices[1][1] + vertices[3][1]);
    mid1[2] = 0.5 * (vertices[1][2] + vertices[3][2]);

    /* 1st decomposition */

    /* mo0 = v1 - mid0 */
    mo0[0] = vertices[1][0] - mid0[0];
    mo0[1] = vertices[1][1] - mid0[1];
    mo0[2] = vertices[1][2] - mid0[2];

    /* mo1 = v3 - mid0 */
    mo1[0] = vertices[3][0] - mid0[0];
    mo1[1] = vertices[3][1] - mid0[1];
    mo1[2] = vertices[3][2] - mid0[2];

    nmo0 = mo0[0] * mo0[0] + mo0[1] * mo0[1] + mo0[2] * mo0[2];
    nmo1 = mo1[0] * mo1[0] + mo1[1] * mo1[1] + mo1[2] * mo1[2];

    if (nmo0 * nmo1 > 0.0)
    {
        denom = DecompositionUtils::getSquareRoot(nmo0 * nmo1);
    }
    else
    {
        denom = 1.0;
    }

    dot0 = (mo0[0] * mo1[0] + mo0[1] * mo1[1] + mo0[2] * mo1[2]) / denom;

    /* 2nd decomposition */

    /* mo0 = v2 - mid1 */
    mo0[0] = vertices[2][0] - mid1[0];
    mo0[1] = vertices[2][1] - mid1[1];
    mo0[2] = vertices[2][2] - mid1[2];

    /* mo1 = v0 - mid1 */
    mo1[0] = vertices[0][0] - mid1[0];
    mo1[1] = vertices[0][1] - mid1[1];
    mo1[2] = vertices[0][2] - mid1[2];

    nmo0 = mo0[0] * mo0[0] + mo0[1] * mo0[1] + mo0[2] * mo0[2];
    nmo1 = mo1[0] * mo1[0] + mo1[1] * mo1[1] + mo1[2] * mo1[2];

    if (nmo0 * nmo1 > 0.0)
    {
        denom = getSquareRoot(nmo0 * nmo1);
    }
    else
    {
        denom = 1.0;
    }

    dot1 = (mo0[0] * mo1[0] + mo0[1] * mo1[1] + mo0[2] * mo1[2]) / denom;

    /* The lower the dot product, the closer to -1, and the more coplanar the triangles are. */
    if (dot0 <= dot1)
    {
        triangleVertexIndices[0] = facetVertexIndices[0];
        triangleVertexIndices[1] = facetVertexIndices[1];
        triangleVertexIndices[2] = facetVertexIndices[2];
        triangleVertexIndices[3] = facetVertexIndices[0];
        triangleVertexIndices[4] = facetVertexIndices[2];
        triangleVertexIndices[5] = facetVertexIndices[3];
    }
    else
    {
        triangleVertexIndices[0] = facetVertexIndices[1];
        triangleVertexIndices[1] = facetVertexIndices[2];
        triangleVertexIndices[2] = facetVertexIndices[3];
        triangleVertexIndices[3] = facetVertexIndices[1];
        triangleVertexIndices[4] = facetVertexIndices[3];
        triangleVertexIndices[5] = facetVertexIndices[0];
    }
}

