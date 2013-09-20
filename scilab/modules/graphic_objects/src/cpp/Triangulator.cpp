/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "Triangulator.hxx"

#include <math.h>

void Triangulator::determineSmallestAxis(void)
{
    double minval = 0.;

    Vector3d min = inputPoints[0];
    Vector3d max = min;

    for (int i = 1 ; i < numPoints; i++)
    {
        if (inputPoints[i].x < min.x)
        {
            min.x = inputPoints[i].x;
        }
        if (inputPoints[i].y < min.y)
        {
            min.y = inputPoints[i].y;
        }
        if (inputPoints[i].z < min.z)
        {
            min.z = inputPoints[i].z;
        }

        if (inputPoints[i].x > max.x)
        {
            max.x = inputPoints[i].x;
        }
        if (inputPoints[i].y > max.y)
        {
            max.y = inputPoints[i].y;
        }
        if (inputPoints[i].z > max.z)
        {
            max.z = inputPoints[i].z;
        }
    }

    max = minus(max, min);

    if (max.x < max.y)
    {
        minval = max.x;
        smallestAxis = 0;
        largestAxes[0] = 1;
        largestAxes[1] = 2;
    }
    else
    {
        minval = max.y;
        smallestAxis = 1;
        largestAxes[0] = 0;
        largestAxes[1] = 2;
    }

    if (max.z < minval)
    {
        smallestAxis = 2;
        largestAxes[0] = 0;
        largestAxes[1] = 1;
    }
}

void Triangulator::fillPoints(void)
{
    Vector3d point;

    for (int i = 0; i < numPoints; i++)
    {
        if (smallestAxis == 0)
        {
            /* Smallest axis: X */
            point.x = inputPoints[i].y;
            point.y = inputPoints[i].z;
        }
        else if (smallestAxis == 1)
        {
            /* Smallest axis: Y */
            point.x = inputPoints[i].x;
            point.y = inputPoints[i].z;
        }
        else
        {
            /* Smallest axis: Z (2) */
            point.x = inputPoints[i].x;
            point.y = inputPoints[i].y;
        }

        point.z = 0.0;

        points.push_back(point);
    }
}

double Triangulator::computeArea(void)
{
    double area = 0.0;

    for (int i = 0; i < numPoints; i++)
    {
        int ip1 = (i + 1) % numPoints;

        area += ((points[i].x * points[ip1].y) - (points[i].y * points[ip1].x));
    }

    area *= 0.5;

    return area;
}

void Triangulator::fillVertexIndices(void)
{
    if (flipped)
    {
        for (int i = numPoints - 1; i >= 0; i--)
        {
            vertexIndices.push_back(i);
        }
    }
    else
    {
        for (int i = 0; i < numPoints; i++)
        {
            vertexIndices.push_back(i);
        }
    }
}

void Triangulator::removeColinearVertices(void)
{
    double dp = 0.;
    std::list<int>::iterator vi, vim1, vip1;

    std::vector<Vector3d> sievedPoints;
    std::list<int> tmpVertexIndices;

    int numColinear = 0;
    int index = 0;

    for (vi = vertexIndices.begin(); vi != vertexIndices.end(); vi++)
    {
        getAdjacentVertices(vi, vim1, vip1);

        dp = computeDotProduct(*vim1, *vi, *vip1);

        if ((!compareVertices(points[*vim1], points[*vi]) && !compareVertices(points[*vi], points[*vip1])) &&
                fabs(dp) < TOLERANCE)
        {
            numColinear++;
        }
        else
        {
            sievedPoints.push_back(points[*vi]);
            actualVertexIndices.push_back(*vi);
        }

        index++;
    }

    points.clear();

    if (flipped)
    {
        /* Reverse the actual vertex indices list */
        std::vector<int> tmpList;

        for (size_t i = 0; i < actualVertexIndices.size(); i++)
        {
            tmpList.push_back(actualVertexIndices[actualVertexIndices.size() - i - 1]);
        }

        actualVertexIndices.clear();

        for (size_t i = 0; i < tmpList.size(); i++)
        {
            actualVertexIndices.push_back(tmpList[i]);
        }

        tmpList.clear();
    }

    if (flipped)
    {
        for (size_t i = 0; i < sievedPoints.size(); i++)
        {
            points.push_back(sievedPoints[sievedPoints.size() - i - 1]);
        }
    }
    else
    {
        for (size_t i = 0; i < sievedPoints.size(); i++)
        {
            points.push_back(sievedPoints[i]);
        }
    }

    /* Must be updated */
    numPoints = (int)points.size();

    sievedPoints.clear();

    numColinearVertices = numColinear;
}

void Triangulator::removeDuplicateVertices(void)
{
    int numDuplicateVertices = 0;
    std::vector<int> duplicateFlagArray;

    std::vector<Vector3d> sievedPoints;
    std::vector<int> tmpActualVertexIndices;

    sievedPoints.clear();
    tmpActualVertexIndices.clear();

    duplicateFlagArray.resize(points.size());

    for (size_t i = 0; i < points.size(); i++)
    {
        int ic = ((int)i+1) % (int)points.size();
        int icm1 = (int)i;

        Vector3d vi = points[icm1];
        Vector3d vip1 = points[ic];

        if (compareVertices(vi, vip1))
        {
            numDuplicateVertices++;
            duplicateFlagArray[ic] = 1;
        }
        else
        {
            duplicateFlagArray[ic] = 0;
        }
    }

    for (size_t i = 0; i < points.size(); i++)
    {
        if (duplicateFlagArray[i] == 0)
        {
            /* Keep as it is not a duplicate */
            sievedPoints.push_back(points[i]);
            tmpActualVertexIndices.push_back(actualVertexIndices[i]);
        }
    }

    actualVertexIndices.clear();
    points.clear();

    /* Copy the two new lists */
    for (size_t i = 0; i < tmpActualVertexIndices.size(); i++)
    {
        actualVertexIndices.push_back(tmpActualVertexIndices[i]);
    }

    for (size_t i = 0; i < sievedPoints.size(); i++)
    {
        points.push_back(sievedPoints[i]);
    }

    /* Must be updated */
    numPoints = (int)points.size();

    duplicateFlagArray.clear();
    sievedPoints.clear();
    tmpActualVertexIndices.clear();
}

void Triangulator::fillConvexVerticesList(void)
{
    std::list<int>::iterator vi;

    flagList.resize(vertexIndices.size());

    for (vi = vertexIndices.begin(); vi != vertexIndices.end(); vi++)
    {
        if (isConvex(vi))
        {
            convexList.push_back(*vi);
            flagList[*vi] = true;
        }
        else
        {
            reflexList.push_back(*vi);
            flagList[*vi] = false;
        }
    }
}

void Triangulator::fillEarList(void)
{
    std::list<int>::iterator vi;
    bool res = false;

    for (vi = vertexIndices.begin(); vi != vertexIndices.end(); vi++)
    {
        if (flagList[*vi])
        {
            res = isAnEar(vi);

            if (res)
            {
                earList.push_back(*vi);
            }
        }
    }
}

void Triangulator::getAdjacentVertices(std::list<int>::iterator vi, std::list<int>::iterator& vim1, std::list<int>::iterator& vip1)
{
    if (*vi == vertexIndices.front())
    {
        vim1 = vertexIndices.end();
        vim1--;
    }
    else
    {
        vim1 = vi;
        vim1--;
    }

    if (*vi == vertexIndices.back())
    {
        vip1 = vertexIndices.begin();
    }
    else
    {
        vip1 = vi;
        vip1++;
    }
}

bool Triangulator::isConvex(std::list<int>::iterator vertex)
{
    double dp = 0.0;
    std::list<int>::iterator pred, succ;

    getAdjacentVertices(vertex, pred, succ);

    dp = computeDotProduct(*pred, *vertex, *succ);

    if (dp >= 0.0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Triangulator::isAnEar(std::list<int>::iterator vertex)
{
    bool isEar = true;
    std::list<int>::iterator pred, succ;
    std::list<int>::iterator vi;

    Vector3d v0, v1, v2;

    getAdjacentVertices(vertex, pred, succ);

    v0 = points[*pred];
    v1 = points[*vertex];
    v2 = points[*succ];

    for (vi = reflexList.begin(); vi != reflexList.end(); vi++)
    {
        if (*vi == *pred || *vi == *vertex || *vi == *succ)
        {
            continue;
        }
        else
        {
            bool res;

            res = pointInTriangle(v0, v1, v2, points[*vi]);

            if (res)
            {
                isEar = false;
                break;
            }
        }

    }

    numEarTests++;

    return isEar;
}

/* To do: streamline */
void Triangulator::updateVertex(std::list<int>::iterator vertex)
{
    bool res = false;

    if (flagList[*vertex])
    {
        /*
         * Convex vertex: remove it from the ear list if not an ear any more,
         * else add it if not already present.
         */
        res = isAnEar(vertex);

        if (!res)
        {
            earList.remove(*vertex);
            numDelEars++;
        }
        else
        {
            std::list<int>::iterator foundEar;

            foundEar = find(earList.begin(), earList.end(), *vertex);

            if (foundEar == earList.end())
            {
                earList.push_front(*vertex);
                numAddEars++;
            }
        }
    }
    else
    {
        /*
         * Non-convex vertex: may become convex, so its flag must be updated, as well
         * as the reflex vertex list.
         * Also determine whether it has become an ear and update the ear list accordingly.
         */
        if (isConvex(vertex))
        {
            flagList[*vertex] = true;
        }

        if (flagList[*vertex])
        {
            res = isAnEar(vertex);

            if (res)
            {
                std::list<int>::iterator foundEar;

                foundEar = find(earList.begin(), earList.end(), *vertex);

                if (foundEar == earList.end())
                {
                    earList.push_front(*vertex);
                    numAddEars++;
                }
            }

            reflexList.remove(*vertex);
        }
    }
}

double Triangulator::computeDotProduct(int im1, int i, int ip1)
{
    double dp = 0.;
    Vector3d eim1p;

    Vector3d eim1 = minus(points[i], points[im1]);
    Vector3d ei = minus(points[ip1], points[i]);

    /* Normalize */
    eim1 = normalize(eim1);
    ei = normalize(ei);

    /* Ought to use cross product */
    eim1p = perpendicularVector(eim1);

    dp = dot(eim1p, ei);

    return dp;
}

bool Triangulator::pointInTriangle(Vector3d A, Vector3d B, Vector3d C, Vector3d P)
{
    double dot00 = 0., dot01 = 0., dot02 = 0., dot11 = 0., dot12 = 0.;
    double invDenom = 0.;
    double u = 0., v = 0.;

    Vector3d v0, v1, v2;

    /* Compute vectors */
    v0 = minus(C, A);
    v1 = minus(B, A);
    v2 = minus(P, A);

    /* Compute dot products */
    dot00 = dot(v0, v0);
    dot01 = dot(v0, v1);
    dot02 = dot(v0, v2);
    dot11 = dot(v1, v1);
    dot12 = dot(v1, v2);

    /* Compute barycentric coordinates */
    invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    /* Check if point is in triangle */
    return (u >= 0) && (v >= 0) && (u + v < 1);
}

Vector3d Triangulator::minus(Vector3d v0, Vector3d v1)
{
    Vector3d res;

    res.x = v0.x - v1.x;
    res.y = v0.y - v1.y;
    res.z = v0.z - v1.z;

    return res;
}

double Triangulator::dot(Vector3d v0, Vector3d v1)
{
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

Vector3d Triangulator::normalize(Vector3d v)
{
    double n = sqrt(v.x * v.x + v.y * v.y);

    if (n < EPSILON)
    {
        n = 1.0;
    }

    v.x /= n;
    v.y /= n;

    return v;
}

Vector3d Triangulator::perpendicularVector(Vector3d v)
{
    Vector3d perp;
    perp.x = -v.y;
    perp.y = v.x;
    perp.z = 0.0;

    return perp;
}

bool Triangulator::compareVertices(Vector3d v0, Vector3d v1)
{
    if (areEqual(v0.x, v1.x) && areEqual(v0.y, v1.y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Triangulator::areEqual(double x0, double x1)
{
    double maxAbs = fabs(x0) > fabs(x1) ? fabs(x0) : fabs(x1);

    if (fabs(x0 - x1) <= EPSILON)
    {
        return true;
    }
    else if (fabs(x0 - x1) <= EPSILON * maxAbs)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Triangulator::Triangulator(void)
{
    numPoints = 0;
    numInitPoints = 0;
    flipped = false;
    numAddEars = 0;
    numDelEars = 0;
    numSteps = 0;
    numEarTests = 0;
    numColinearVertices = 0;

    inputPoints.clear();
    points.clear();
    vertexIndices.clear();
    actualVertexIndices.clear();
    earList.clear();
    convexList.clear();
    reflexList.clear();
    flagList.clear();
    triangleIndices.clear();
}

void Triangulator::initialize(void)
{
    double area = 0.;

    numPoints = (int)inputPoints.size();

    determineSmallestAxis();
    fillPoints();
    area = computeArea();

    if (area < 0.0)
    {
        flipped = true;
    }
    else
    {
        flipped = false;
    }

    fillVertexIndices();

    numInitPoints = numPoints;

    /*
     * An additional colinear vertices removal pass should be performed
     * after the duplicate removal pass, as some adjacent edges may become colinear.
     */
    removeColinearVertices();
    removeDuplicateVertices();

    /* Vertex indices must be re-filled */
    vertexIndices.clear();
    fillVertexIndices();

    fillConvexVerticesList();
    fillEarList();
}

void Triangulator::addPoint(double x, double y, double z)
{
    Vector3d point;

    point.x = x;
    point.y = y;
    point.z = z;

    inputPoints.push_back(point);
}

void Triangulator::triangulate(void)
{
    int triIndex = 0;
    std::list<int>::iterator it;
    std::list<int>::iterator vertex, pred, succ;

    numSteps = 0;

    while (vertexIndices.size() >= 3 && earList.size() > 0)
    {
        int v0 = 0, v1 = 0, v2 = 0;
        int v0actual = 0, v1actual = 0, v2actual = 0;
        int vertexIndex = 0;

        it = earList.begin();

        /* If not found, we should break out of the loop. To be checked. */
        vertex = find(vertexIndices.begin(), vertexIndices.end(), *it);
        vertexIndex = *vertex;

        getAdjacentVertices(vertex, pred, succ);

        /* Remove */
        vertexIndices.remove(*vertex);
        earList.pop_front();

        numDelEars++;


        triIndex = *pred;
        v0 = triIndex;
        v1 = vertexIndex;
        triIndex = *succ;
        v2 = triIndex;

        v0actual = actualVertexIndices[v0];
        v1actual = actualVertexIndices[v1];
        v2actual = actualVertexIndices[v2];

        triangleIndices.push_back(v0actual);
        triangleIndices.push_back(v1actual);
        triangleIndices.push_back(v2actual);

        /* Update the predecessor vertex */
        updateVertex(pred);

        /* Update the successor vertex */
        updateVertex(succ);

        numSteps++;
    }
}

int Triangulator::getNumberTriangles(void)
{
    return (int)(triangleIndices.size() / 3);
}

int* Triangulator::getIndices(void)
{
    return &triangleIndices[0];
}

int Triangulator::getNumberSteps(void)
{
    return numSteps;
}

int Triangulator::getNumberEarTests(void)
{
    return numEarTests;
}

void Triangulator::clear(void)
{
    inputPoints.clear();
    points.clear();
    numPoints = 0;
    numInitPoints = 0;

    vertexIndices.clear();
    actualVertexIndices.clear();
    earList.clear();
    convexList.clear();
    reflexList.clear();
    flagList.clear();
    triangleIndices.clear();

    numAddEars = 0;
    numDelEars = 0;
    numSteps = 0;
    numEarTests = 0;
    numColinearVertices = 0;
}

