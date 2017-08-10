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

#ifndef TRIANGULATOR_HXX
#define TRIANGULATOR_HXX

#include <algorithm>
#include <limits>
#include <list>
#include <vector>

#include <iostream>

/*
 * A structure representing a point.
 */
struct Vector3d
{
    double x;
    double y;
    double z;

    Vector3d() : x(0.), y(0.), z(0.) { }
    Vector3d(const double _x, const double _y, const double _z) : x(_x), y(_y), z(_z) { }
};

/**
 * Triangulator class
 * An implementation of the ear-clipping triangulation algorithm,
 * an O(n^2) complexity algorithm, when n is the triangulated polygon's
 * number of points. The polygon must be simple and non-intersecting.
 * Triangulation occurs as if the polygon were plane, which is why only
 * its two largest dimensions are considered and the third is ignored.
 *
 * To do:
 *    -extend to take into account self-intersecting polygons
 *    -use a more efficient and robust algorithm (such as the decomposition into monotone pieces O(n log n) algorithm)
 *    -use a dedicated library (more efficient and/or robust)
 */
class Triangulator
{
private:
    /** The array of input points. */
    std::vector<Vector3d> inputPoints;

    /**
     * The array of points, filled from the array of input points depending on the polygon's dimensions.
     * It contains exactly the same number of points.
     */
    std::vector<Vector3d> points;

    /** The polygon's number of points. */
    int numPoints;

    /** The polygons's initial number of points, including colinear vertices. */
    int numInitPoints;

    /**
     * Specifies whether the list of vertex indices must be flipped or not
     * and indicates the vertices' order. If false, vertices are ordered counter-clockwise
     * whereas they are ordered clockwise if it is true.
     */
    bool flipped;

    /** The list of vertex indices. */
    std::list<int> vertexIndices;

    /** The list of actual vertex indices. */
    std::vector<int> actualVertexIndices;

    /** The list of ear vertex indices. */
    std::list<int> earList;

    /** The list of convex vertices. */
    std::list<int> convexList;

    /** The list of reflex vertices. */
    std::list<int> reflexList;

    /** The convexity flag array. */
    std::vector<bool> flagList;

    /** The list of output triangle indices. */
    std::vector<int> triangleIndices;

    /** The number of insertions into the ear vertex list. */
    int numAddEars;

    /** The number of deletions from the ear vertex list. */
    int numDelEars;

    /** The number of steps taken by the triangulation's execution. */
    int numSteps;

    /** The number of ear tests performed. */
    int numEarTests;

    /** The number of colinear vertices. */
    int numColinearVertices;

    double xmin, xmax, ymin, ymax, zmin, zmax;

private:

    /**
     * Fills the array of points with a projection of the points in the array of input points.
     **/
    void fillPoints(void);

    /**
     * Computes and returns the polygon's signed area.
     * Its sign depends on the order of the polygon's vertices.
     * If positive, the vertices are ordered counter-clockwise whereas they are ordered
     * clockwise if it is negative.
     * @return the polygon's area.
     */
    double computeArea(void);

    /**
     * Fills the list of vertex indices, depending on their order.
     */
    void fillVertexIndices(void);

    /**
     * Removes colinear vertices.
     */
    void removeColinearVertices(void);

    /**
     * Removes duplicate vertices.
     */
    void removeDuplicateVertices(void);

    /**
     * Fills the list of convex vertices, determining whether each vertex vi is convex or not.
     * It also fills the convexity flag array.
     */
    void fillConvexVerticesList(void);

    /**
     * Fills the list of ear vertices.
     */
    void fillEarList(void);

    /**
     * Gets the vertices adjacent to vertex i.
     * @param[in] vertex i's iterator.
     * @param[out] a reference to vertex i-1's iterator.
     * @param[out] a reference to vertex i+1's iterator.
     */
    void getAdjacentVertices(std::list<int>::iterator vi, std::list<int>::iterator& vim1, std::list<int>::iterator& vip1);

    /**
     * Determines whether a vertex is convex or not.
     * @param[in] the vertex's iterator.
     * @return true if the vertex is convex, false if it is not.
     */
    bool isConvex(std::list<int>::iterator vertex);

    /**
     * Determines whether a vertex is an ear vertex.
     * @param[in] the vertex's iterator.
     * @return true if it is an ear, false if not.
     */
    bool isAnEar(std::list<int>::iterator vertex);

    /**
     * Updates a vertex's state due to the next/previous vertex
     * having been removed. It performs an ear test and accordingly
     * updates the ear vertex list. If the vertex becomes convex, its
     * flag is updated as well as the reflex vertex list.
     * @param[in] the updated vertex's iterator.
     */
    void updateVertex(std::list<int>::iterator vertex);

    /**
     * Computes the dot product between edge e(i) (from vertex i to i+1) and the vector
     * orthogonal to edge e(i-1) (from vertex i-1 to i) such that the angle from e(i-1)
     * to this vector vector is +90°.
     * @param[in] the index of vertex i-1.
     * @param[in] the index of vertex i.
     * @param[in] the index of vertex i+1.
     * @return the dot product.
     */
    double computeDotProduct(int im1, int i, int ip1);

    /**
     * Determines whether a point P is located within the triangle ABC.
     * It considers that P and ABC both lie in the xy plane.
     * @param[in] the triangle's first point.
     * @param[in] the triangle's second point.
     * @param[in] the triangle's third point.
     * @return true if P is located within ABC, false if not.
     */
    bool pointInTriangle(Vector3d A, Vector3d B, Vector3d C, Vector3d P);

    /**
     * Subtracts the second input vector from the first one and returns the result.
     * It should be moved to a Vector3d class.
     * @param[in] the first vector.
     * @param[in] the second vector.
     * @return the resulting vector.
     */
    static Vector3d minus(Vector3d v0, Vector3d v1);

    /**
    * Add the first and second vectors
    * It should be moved to a Vector3d class.
    * @param[in] the first vector.
    * @param[in] the second vector.
    * @return the resulting vector.
    */
    static Vector3d plus(Vector3d v0, Vector3d v1);

    /**
     * Computes and returns the dot product of two vectors.
     * It should be moved to a Vector3d class.
     * @param[in] the first vector.
     * @param[in] the second vector.
     * @return the dot product.
     */
    static double dot(Vector3d v0, Vector3d v1);

    /**
     * Normalizes a 2D vector, the z coordinate is ignored.
     * It should be moved to a Vector3d class.
     * @param[in] the vector to normalize.
     * @return the normalized vector.
     */
    static Vector3d normalize(Vector3d v);

    /**
     * Compares whether two vertices are identical or not.
     * Two vertices are considered identical if they x and y coordinates
     * are equal, the z coordinate being ignored.
     * It should be moved to a Vector3d class.
     * @param[in] the first vector.
     * @param[in] the second vector.
     * @return true if the two vertices are identical, false if not.
     */
    static bool compareVertices(Vector3d v0, Vector3d v1);

    /**
     * Determines whether two floating-point values are equal.
     * @param[in] the first value.
     * @param[in] the second value.
     * @return true if the values are equal, false if not.
     */
    static bool areEqual(double x0, double x1);

    /**
     * Computes and returns a vector p orthogonal to vector v,
     * such that the angle from v to p is +90°. v and p are considered
     * to lie in the xy plane.
     * It should be moved to a Vector3d class.
     * @param[in] the vector.
     * @return the vector perpendicular to vector v.
     */
    static Vector3d perpendicularVector(Vector3d v);

    /**
     * Computes and returns the cross product of two vectors.
     * It should be moved to a Vector3d class.
     * @param[in] the vector a.
     * @param[in] the vector b.
     * @return the cross product vector perpendicular to vectors a and b.
     */
    static Vector3d cross(Vector3d a, Vector3d b);

    /**
     * Computes the multimplication of 3x3 matrix Out = A * B;
     * @param[in] the left matrix a.
     * @param[in] the right matrix b.
     * @param[out] the multiplied matrix.
     **/
    void matrixMatrixMul(double (&a)[3][3], double (&b)[3][3], double (&out)[3][3]);

    /**
     * Computes the multimplication of 3x3 matrix and a vector Pout = M * Pin;
     * @param[in] the matrix m.
     * @param[in] the vector pin.
     * @param[out] the multiplied vector pout.
     **/
    void matrixVectorMul(double (&m)[3][3], Vector3d & pin, Vector3d & pout);

    /**
     * Computes the multimplication of 3x3 matrix and a vector PinOut = M * PinOut;
     * @param[in] the matrix m.
     * @param[in] the vector pinOut.
     * @param[out] the multiplied vector pinOut.
     **/
    void matrixVectorMul(double (&m)[3][3], Vector3d & pinOut);

    /**
     * Transform all points from the array of points, to the cube [0,1]^3
     **/
    void normalizePoints(void);

    /**
     * 3x3 Matriz diagonalizer using jacobi method
     * Original idea from:  http://www.melax.com/diag.html
     * @param[in] the matrix A to be diagonalized.
     * @param[out] the matrix Q which diagonalizer A, D = Q' * D * Q.
     * @param[out] the matrix D diagonal.
     **/
    void diagonalize(const double (&A)[3][3], double (&Q)[3][3], double (&D)[3][3]);

public:
    /** Default constructor. */
    Triangulator(void);

    /**
     * Initializes and fills the lists used by the algorithm from the
     * list of input points, which must have been filled beforehand.
     */
    void initialize(void);

    /**
     * Adds a point to the array of input points.
     * @param[in] the point's x-coordinate.
     * @param[in] the point's y-coordinate.
     * @param[in] the point's z-coordinate.
     */
    void addPoint(double x, double y, double z);

    /**
     * Triangulates the input data.
     */
    void triangulate(void);

    /**
     * Returns the number of triangles resulting from the algorithm's execution.
     * @return the number of triangles.
     */
    int getNumberTriangles(void);

    /**
     * Returns the array of triangle indices resulting from the algorithm's execution.
     * @return a pointer to the array of triangle indices.
     */
    int* getIndices(void);

    /**
     * Returns the number steps performed by the algorithm.
     * @return the number of steps performed.
     */
    int getNumberSteps();

    /**
     * Returns the number of ear tests performed by the algorithm.
     * Relevant only to debugging.
     * @return the number of ear tests performed.
     */
    int getNumberEarTests();

    /**
     * Clears the lists of points, vertex indices, and the lists storing
     * the algorithm's internal state (vertex indices, ear vertices and others).
     */
    void clear(void);
};

/**
 * An arbitrary tolerance value used to determine colinear edges.
 */
#define TOLERANCE 0.0000001

/**
 * An epsilon value used when comparing vertices.
 */
#define EPSILON 0.00000001

#endif
