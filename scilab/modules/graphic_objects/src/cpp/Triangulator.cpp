/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "Triangulator.hxx"

void Triangulator::determineSmallestAxis(void)
{
    double minval = 0.;

    Vector3d min = inputPoints[0];
    Vector3d max = min;

    for (int i = 1 ; i < numPoints; i++)
    {
        if (inputPoints[i].x < min.x) min.x = inputPoints[i].x;
        if (inputPoints[i].y < min.y) min.y = inputPoints[i].y;
        if (inputPoints[i].z < min.z) min.z = inputPoints[i].z;

        if (inputPoints[i].x > max.x) max.x = inputPoints[i].x;
        if (inputPoints[i].y > max.y) max.y = inputPoints[i].y;
        if (inputPoints[i].z > max.z) max.z = inputPoints[i].z;
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
        int ip1 = (i+1) % numPoints;

        area += ((points[i].x*points[ip1].y)-(points[i].y*points[ip1].x));
    }

    area *= 0.5;

    return area;
}

void Triangulator::fillVertexIndices(void)
{
    if (flipped)
    {
        for (int i = numPoints-1; i >= 0; i--)
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

void Triangulator::fillConvexVerticesList(void)
{
    double dp = 0.;
    std::list<int>::iterator vi, vim1, vip1;

    flagList.resize(vertexIndices.size());

    for (vi = vertexIndices.begin(); vi != vertexIndices.end(); vi++)
    {
        getAdjacentVertices(vi, vim1, vip1);

        dp = computeDotProduct(*vim1, *vi, *vip1);

        if (dp >= 0.0)
        {
            convexList.push_back(*vi);
            flagList[*vi] = 1;
        }
        else
        {
            reflexList.push_back(*vi);
            flagList[*vi] = 0;
        }
    }
}

void Triangulator::fillEarList(void)
{
    std::list<int>::iterator vi;
    int res = 0;

    for (vi = vertexIndices.begin(); vi != vertexIndices.end(); vi++)
    {
        std::list<int>::iterator vim1, vip1;

        getAdjacentVertices(vi, vim1, vip1);

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

/*
 * To do: optimize by iterating only over the reflex vertex list.
 */
int Triangulator::isAnEar(std::list<int>::iterator vertex)
{
    int isEar = 1;
    std::list<int>::iterator pred, succ;
    std::list<int>::iterator vi;

    Vector3d v0, v1, v2;

    getAdjacentVertices(vertex, pred, succ);

    v0 = points[*pred];
    v1 = points[*vertex];
    v2 = points[*succ];

    for (vi = vertexIndices.begin(); vi != vertexIndices.end(); vi++)
    {
        if (*vi == *pred || *vi == *vertex || *vi == *succ)
        {
            continue;
        }
        else
        {
            int res;

            res = pointInTriangle(v0, v1, v2, points[*vi]);

            if (res == 1)
            {
                isEar = 0;
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
    double dp = 0.;
    int res = 0;

    std::list<int>::iterator pred, succ;

    if (flagList[*vertex])
    {
        /*
         * Convex vertex: remove it from the ear list if not an ear any more,
         * else add it if not already present.
         */
        res = isAnEar(vertex);

        if (res == 0)
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
         * Non-convex vertex: may become convex, so its flag must be updated.
         * Also determine whether it has become an ear and update the ear list accordingly.
         */
        getAdjacentVertices(vertex, pred, succ);

        dp = computeDotProduct(*pred, *vertex, *succ);

        if (dp >= 0.0)
        {
            flagList[*vertex] = 1;
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

        }
    }
}

double Triangulator::computeDotProduct(int im1, int i, int ip1)
{
    double dp = 0.;
    Vector3d eim1 = minus(points[i], points[im1]);
    Vector3d ei = minus(points[ip1], points[i]);
    Vector3d eim1p = perpendicularVector(eim1);
    /* Ought to use cross product */
    dp = dot(eim1p, ei);

    return dp;
}

int Triangulator::pointInTriangle(Vector3d A, Vector3d B, Vector3d C, Vector3d P)
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
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

Vector3d Triangulator::perpendicularVector(Vector3d v)
{
    Vector3d perp;
    perp.x = -v.y;
    perp.y = v.x;
    perp.z = 0.0;

    return perp;
}

Triangulator::Triangulator(void)
{
    numPoints = 0;
    flipped = false;
    numAddEars = 0;
    numDelEars = 0;
    numSteps = 0;
    numEarTests = 0;
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
        it = earList.begin();

        /* If not found, we should break out of the loop. To be checked. */
        vertex = find(vertexIndices.begin(), vertexIndices.end(), *it);

        getAdjacentVertices(vertex, pred, succ);

        /* Remove */
        vertexIndices.remove(*vertex);
        earList.pop_front();

        numDelEars++;

        triIndex = *pred;
        triangleIndices.push_back(triIndex);
        triIndex = *vertex;
        triangleIndices.push_back(triIndex);
        triIndex = *succ;
        triangleIndices.push_back(triIndex);

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

    vertexIndices.clear();
    earList.clear();
    convexList.clear();
    reflexList.clear();
    flagList.clear();
    triangleIndices.clear();
}
