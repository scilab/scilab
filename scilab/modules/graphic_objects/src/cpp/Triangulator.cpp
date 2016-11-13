/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2012 - DIGITEO - Manuel Juliachs
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

#include "Triangulator.hxx"

#include <math.h>


void Triangulator::fillPoints(void)
{
    points.resize(numPoints, Vector3d(0., 0., 0.));

    //If any coordinate is constant project on the corresponding plane
    if (xmin == xmax)
    {
        for (int i = 0; i < numPoints; i++)
        {
            points[i].x = inputPoints[i].z;
            points[i].y = inputPoints[i].y;
        }
    }
    else if (ymin == ymax)
    {
        for (int i = 0; i < numPoints; i++)
        {
            points[i].x = inputPoints[i].x;
            points[i].y = inputPoints[i].z;
        }
    }
    else if (zmin == zmax)
    {
        for (int i = 0; i < numPoints; i++)
        {
            points[i].x = inputPoints[i].x;
            points[i].y = inputPoints[i].y;
        }
    }
    //Otherwise, find the plane that best fit the data and rotate it to the xy plane
    else
    {
        //m covariance matrix
        //q autovector matrix
        //d diagonal matrix
        double m[3][3] = {{0., 0., 0.}, {0., 0., 0.}, {0., 0., 0.}};
        //'q' and 'd' are not needed to be initialized
        // since they are output paraments, but zero-initialize
        // to follow coverity
        double q[3][3] = {{0., 0., 0.}, {0., 0., 0.}, {0., 0., 0.}};
        double d[3][3] = {{0., 0., 0.}, {0., 0., 0.}, {0., 0., 0.}};
        Vector3d middle(0., 0., 0.);
        Vector3d approxNormal(0., 0., 0.);

        for (int i = 0; i < numPoints; i++)
        {
            middle.x += inputPoints[i].x;
            middle.y += inputPoints[i].y;
            middle.z += inputPoints[i].z;
            Vector3d tmp = cross(minus(inputPoints[(i + 1) % numPoints], inputPoints[i]), minus(inputPoints[(i + 1) % numPoints], inputPoints[(i + 2) % numPoints]));
            approxNormal = plus(approxNormal, tmp);
        }

        double den = sqrt(approxNormal.x * approxNormal.x + approxNormal.y * approxNormal.y + approxNormal.z * approxNormal.z);

        approxNormal.x /= den;
        approxNormal.y /= den;
        approxNormal.z /= den;

        middle.x /= numPoints;
        middle.y /= numPoints;
        middle.z /= numPoints;

        for (int i = 0; i < numPoints; ++i)
        {
            points[i].x = inputPoints[i].x - middle.x;
            points[i].y = inputPoints[i].y - middle.y;
            points[i].z = inputPoints[i].z - middle.z;

            m[0][0] += points[i].x * points[i].x;
            m[0][1] += points[i].x * points[i].y;
            m[0][2] += points[i].x * points[i].z;
            m[1][1] += points[i].y * points[i].y;
            m[1][2] += points[i].y * points[i].z;
            m[2][2] += points[i].z * points[i].z;
        }


        m[1][0] = m[0][1];
        m[2][0] = m[0][2];
        m[2][1] = m[1][2];

        diagonalize(m, q, d);

        int col = d[0][0] < d[1][1] ? (d[0][0] < d[2][2] ? 0 : 2) : (d[1][1] < d[2][2] ? 1 : 2);

        Vector3d normal(q[0][col], q[1][col], q[2][col]);

        // if the angle between approxNomal and normal is
        // larger than +/- pi/2, we invert the normal and keep the
        // triangle orientation correct
        double sign = dot(approxNormal, normal) < 0. ? -1. : 1.;

        normal.x *= sign;
        normal.y *= sign;
        normal.z *= sign;

        double c, s;

        c = normal.y / sqrt(normal.x * normal.x + normal.y * normal.y);
        s = sqrt(1. - c * c);
        s = normal.x < 0. ? -s : s;

        double rotz[3][3] = {{c, -s, 0.}, {s, c, 0.}, {0., 0., 1.}};

        matrixVectorMul(rotz, normal);

        c = normal.z / sqrt(normal.y * normal.y + normal.z * normal.z);
        s = sqrt(1. - c * c);
        s = normal.y < 0. ? s : -s;

        double rotx[3][3] = {{1., 0., 0.}, {0., c, s}, {0., -s, c}};
        double composedRot[3][3] = {{0., 0., 0.}, {0., 0., 0.}, {0., 0., 0.}};
        matrixMatrixMul(rotx, rotz, composedRot);

        xmin = ymin = zmin = std::numeric_limits<double>::max();
        xmax = ymax = zmax = std::numeric_limits<double>::lowest();

        for (int i = 0; i < numPoints; ++i)
        {
            matrixVectorMul(composedRot, inputPoints[i], points[i]);

            xmin = xmin < points[i].x ? xmin : points[i].x;
            ymin = ymin < points[i].y ? ymin : points[i].y;
            zmin = zmin < points[i].z ? zmin : points[i].z;

            xmax = xmax > points[i].x ? xmax : points[i].x;
            ymax = ymax > points[i].y ? ymax : points[i].y;
            zmax = zmax > points[i].z ? zmax : points[i].z;

            points[i].z = 0.;
        }
    }
}

double Triangulator::computeArea(void)
{
    double area = 0.;

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
        int ic = ((int)i + 1) % (int)points.size();
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
    double dp = 0.;
    std::list<int>::iterator pred, succ;

    getAdjacentVertices(vertex, pred, succ);

    dp = computeDotProduct(*pred, *vertex, *succ);

    if (dp >= 0.)
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
        n = 1.;
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
    perp.z = 0.;

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

    xmin = ymin = zmin = std::numeric_limits<double>::max();
    xmax = ymax = zmax = std::numeric_limits<double>::lowest();

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

    fillPoints();
    normalizePoints();
    area = computeArea();

    if (area < 0.)
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
    Vector3d point(x, y, z);

    if (x < xmin)
    {
        xmin = x;
    }
    if (x > xmax)
    {
        xmax = x;
    }

    if (y < ymin)
    {
        ymin = y;
    }
    if (y > ymax)
    {
        ymax = y;
    }

    if (z < zmin)
    {
        zmin = z;
    }
    if (z > zmax)
    {
        zmax = z;
    }

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

void Triangulator::matrixMatrixMul(double(&a)[3][3], double(&b)[3][3], double(&out)[3][3])
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            out[i][j] = 0.;
            for (int k = 0; k < 3; ++k)
            {
                out[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void Triangulator::matrixVectorMul(double(&m)[3][3], Vector3d & pin, Vector3d & pout)
{
    pout.x = pin.x * m[0][0] + pin.y * m[0][1] + pin.z * m[0][2];
    pout.y = pin.x * m[1][0] + pin.y * m[1][1] + pin.z * m[1][2];
    pout.z = pin.x * m[2][0] + pin.y * m[2][1] + pin.z * m[2][2];
}

void Triangulator::matrixVectorMul(double(&m)[3][3], Vector3d & pinOut)
{
    Vector3d copy(pinOut.x, pinOut.y, pinOut.z);
    pinOut.x = copy.x * m[0][0] + copy.y * m[0][1] + copy.z * m[0][2];
    pinOut.y = copy.x * m[1][0] + copy.y * m[1][1] + copy.z * m[1][2];
    pinOut.z = copy.x * m[2][0] + copy.y * m[2][1] + copy.z * m[2][2];
}

void Triangulator::normalizePoints()
{
    double xscale = xmax == xmin ? 1. : 1. / (xmax - xmin);
    double yscale = ymax == ymin ? 1. : 1. / (ymax - ymin);
    double zscale = zmax == zmin ? 1. : 1. / (zmax - zmin);
    for (int i = 0; i < numPoints; ++i)
    {
        points[i].x = (points[i].x - xmin) * xscale;
        points[i].y = (points[i].y - ymin) * yscale;
        points[i].z = (points[i].z - zmin) * zscale;
    }
}

Vector3d Triangulator::cross(Vector3d a, Vector3d b)
{
    return Vector3d(
               a.y * b.z - b.y * a.z,
               a.z * b.x - a.x * b.z,
               a.x * b.y - a.y * b.x
           );
}

Vector3d Triangulator::plus(Vector3d v0, Vector3d v1)
{
    return Vector3d(v0.x + v1.x, v0.y + v1.y, v0.z + v1.z);
}

void Triangulator::diagonalize(const double (&A)[3][3], double (&Q)[3][3], double (&D)[3][3])
{
    // A must be a symmetric matrix.
    // returns Q and D such that
    // Diagonal matrix D = QT * A * Q;  and  A = Q*D*QT
    const int maxsteps = 24;  // certainly wont need that many.
    double q[4] = {0., 0., 0., 1.};
    for (int i = 0; i < maxsteps; ++i)
    {
        // quat to matrix
        double sqx = q[0] * q[0];
        double sqy = q[1] * q[1];
        double sqz = q[2] * q[2];
        double sqw = q[3] * q[3];
        Q[0][0] = (sqx - sqy - sqz + sqw);
        Q[1][1] = (-sqx + sqy - sqz + sqw);
        Q[2][2] = (-sqx - sqy + sqz + sqw);
        double tmp1 = q[0] * q[1];
        double tmp2 = q[2] * q[3];
        Q[1][0] = 2. * (tmp1 + tmp2);
        Q[0][1] = 2. * (tmp1 - tmp2);
        tmp1 = q[0] * q[2];
        tmp2 = q[1] * q[3];
        Q[2][0] = 2. * (tmp1 - tmp2);
        Q[0][2] = 2. * (tmp1 + tmp2);
        tmp1 = q[1] * q[2];
        tmp2 = q[0] * q[3];
        Q[2][1] = 2. * (tmp1 + tmp2);
        Q[1][2] = 2. * (tmp1 - tmp2);

        double AQ[3][3];
        // AQ = A * Q
        AQ[0][0] = Q[0][0] * A[0][0] + Q[1][0] * A[0][1] + Q[2][0] * A[0][2];
        AQ[0][1] = Q[0][1] * A[0][0] + Q[1][1] * A[0][1] + Q[2][1] * A[0][2];
        AQ[0][2] = Q[0][2] * A[0][0] + Q[1][2] * A[0][1] + Q[2][2] * A[0][2];
        AQ[1][0] = Q[0][0] * A[0][1] + Q[1][0] * A[1][1] + Q[2][0] * A[1][2];
        AQ[1][1] = Q[0][1] * A[0][1] + Q[1][1] * A[1][1] + Q[2][1] * A[1][2];
        AQ[1][2] = Q[0][2] * A[0][1] + Q[1][2] * A[1][1] + Q[2][2] * A[1][2];
        AQ[2][0] = Q[0][0] * A[0][2] + Q[1][0] * A[1][2] + Q[2][0] * A[2][2];
        AQ[2][1] = Q[0][1] * A[0][2] + Q[1][1] * A[1][2] + Q[2][1] * A[2][2];
        AQ[2][2] = Q[0][2] * A[0][2] + Q[1][2] * A[1][2] + Q[2][2] * A[2][2];
        // D = Qt * AQ
        D[0][0] = AQ[0][0] * Q[0][0] + AQ[1][0] * Q[1][0] + AQ[2][0] * Q[2][0];
        D[0][1] = AQ[0][0] * Q[0][1] + AQ[1][0] * Q[1][1] + AQ[2][0] * Q[2][1];
        D[0][2] = AQ[0][0] * Q[0][2] + AQ[1][0] * Q[1][2] + AQ[2][0] * Q[2][2];
        D[1][0] = AQ[0][1] * Q[0][0] + AQ[1][1] * Q[1][0] + AQ[2][1] * Q[2][0];
        D[1][1] = AQ[0][1] * Q[0][1] + AQ[1][1] * Q[1][1] + AQ[2][1] * Q[2][1];
        D[1][2] = AQ[0][1] * Q[0][2] + AQ[1][1] * Q[1][2] + AQ[2][1] * Q[2][2];
        D[2][0] = AQ[0][2] * Q[0][0] + AQ[1][2] * Q[1][0] + AQ[2][2] * Q[2][0];
        D[2][1] = AQ[0][2] * Q[0][1] + AQ[1][2] * Q[1][1] + AQ[2][2] * Q[2][1];
        D[2][2] = AQ[0][2] * Q[0][2] + AQ[1][2] * Q[1][2] + AQ[2][2] * Q[2][2];
        double o[3] = {D[1][2], D[0][2], D[0][1]};
        double m[3] = {fabs(o[0]), fabs(o[1]), fabs(o[2])};

        int k0 = (m[0] > m[1] && m[0] > m[2]) ? 0 : (m[1] > m[2]) ? 1 : 2; // index of largest element of offdiag
        int k1 = (k0 + 1) % 3;
        int k2 = (k0 + 2) % 3;
        if (o[k0] == 0.)
        {
            break;  // diagonal already
        }
        double thet = (D[k2][k2] - D[k1][k1]) / (2. * o[k0]);
        double sgn = (thet > 0.) ? 1. : -1.;
        thet *= sgn; // make it positive
        double t = sgn / (thet + ((thet < 1.E6) ? sqrt(thet * thet + 1.) : thet)); // sign(T)/(|T|+sqrt(T^2+1))
        double c = 1. / sqrt(t * t + 1.); //  c= 1/(t^2+1) , t=s/c
        if (c == 1.)
        {
            break;  // no room for improvement - reached machine precision.
        }
        double jr[4] = {0., 0., 0., 0.};
        jr[k0] = sgn * sqrt((1. - c) / 2.); // using 1/2 angle identity sin(a/2) = sqrt((1-cos(a))/2)
        jr[k0] *= -1.; // since our quat-to-matrix convention was for v*M instead of M*v
        jr[3] = sqrt(1.f - jr[k0] * jr[k0]);
        if (jr[3] == 1.)
        {
            break; // reached limits of floating point precision
        }
        q[0] = (q[3] * jr[0] + q[0] * jr[3] + q[1] * jr[2] - q[2] * jr[1]);
        q[1] = (q[3] * jr[1] - q[0] * jr[2] + q[1] * jr[3] + q[2] * jr[0]);
        q[2] = (q[3] * jr[2] + q[0] * jr[1] - q[1] * jr[0] + q[2] * jr[3]);
        q[3] = (q[3] * jr[3] - q[0] * jr[0] - q[1] * jr[1] - q[2] * jr[2]);
        double mq = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
        q[0] /= mq;
        q[1] /= mq;
        q[2] /= mq;
        q[3] /= mq;
    }
}
