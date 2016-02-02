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

#ifndef DECOMPOSITION_UTILS_HXX
#define DECOMPOSITION_UTILS_HXX

/**
 * Decomposition utility class
 * A set of various static utility methods.
 */

class DecompositionUtils
{

public :

    /**
     * Tests whether a scalar value is a valid number,
     * that is, not a NaN.
     * @param[in] the scalar value to test.
     * @return 1 if the value is not a Nan, 0 if it is.
     */
    static int isANumber(double x);

    static int isANumber(int x)
    {
        return 1;
    }

    static int isANumber(unsigned int x)
    {
        return 1;
    }

    static int isANumber(char x)
    {
        return 1;
    }

    static int isANumber(unsigned char x)
    {
        return 1;
    }

    static int isANumber(short x)
    {
        return 1;
    }

    static int isANumber(unsigned short x)
    {
        return 1;
    }

    /**
     * Tests whether a scalar value is finite.
     * @param[in] the scalar value to test.
     * @return 1 if the value is finite, 0 if it is infinite.
     */
    static int isFinite(double x);

    /**
     * Tests whether a scalar value is valid.
     * The value is valid if it is neither infinite nor a NaN.
     * @param[in] the scalar value to test.
     * @return 1 if the value is valid, 0 if it is not.
     */
    static int isValid(double x);

    /**
     * Tests whether a point is valid.
     * The point is valid if none of its coordinates is infinite or a NaN.
     * @param[in] the point's x-coordinate.
     * @param[in] the point's y-coordinate.
     * @param[in] the point's z-coordinate.
     * @return 1 if the point is valid, 0 if it is not.
     */
    static int isValid(double x, double y, double z);

    /**
     * Tests whether a point is valid.
     * The point is valid if none of its coordinates is infinite or a NaN.
     * @param[in] the point's x-coordinate.
     * @param[in] the point's y-coordinate.
     * @return 1 if the point is valid, 0 if it is not.
     */
    static int isValid(double x, double y);

    /**
     * Returns the base-10 logarithm of the input value.
     * @param[in] the input value.
     * @return the base-10 logarithm of the input value.
     */
    static double getLog10Value(double value);

    /**
     * Determines if a single-component point is valid in logarithmic scale.
     * @param[in] the point's coordinate.
     * @return 1 if the point is valid, 0 if it is not.
     */
    static int isLogValid(double x);

    /**
     * Determines if a point is valid in logarithmic scale.
     * It checks whether any of its coordinates is strictly less than 0 (if the
     * logarithmic scale applies). In the event of the former, the point is
     * determined as not valid.
     * @param[in] the point's x-coordinate.
     * @param[in] the point's y-coordinate.
     * @param[in] the bit mask specifying for which coordinates the logarithmic scale is used.
     * @return 1 if the point is valid, 0 if it is not.
     */
    static int isLogValid(double x, double y, int logMask);

    /**
     * Determines if a point is valid in logarithmic scale.
     * It checks whether any of its coordinates is strictly less than 0 (if the
     * logarithmic scale applies). In the event of the former, the point is
     * determined as not valid.
     * @param[in] the point's x-coordinate.
     * @param[in] the point's y-coordinate.
     * @param[in] the point's z-coordinate.
     * @param[in] the bit mask specifying for which coordinates the logarithmic scale is used.
     * @return 1 if the point is valid, 0 if it is not.
     */
    static int isLogValid(double x, double y, double z, int logMask);

    /**
     * Returns the maximum representable double value.
     * @return the maximum representable double value.
     */
    static double getMaxDoubleValue(void);

    /**
     * Returns the minimum representable double value.
     * @return the minimum representable double value.
     */
    static double getMinDoubleValue(void);

    /**
     * Returns the absolute value of its input value.
     * @param[in] the value.
     * @return the absolute value.
     */
    static double getAbsoluteValue(double value);

    /**
     * Returns the square root of its input value.
     * @param[in] the value.
     * @return the value's square root.
     */
    static double getSquareRoot(double value);

    /**
     * Utility function which outputs the triangle indices of a rectangle decomposed
     * into 2 adjacent triangles. Decomposition is always performed the same way as it does not
     * depend on input vertex values. This function should therefore be used only for rectangles.
     * @param[out] the triangle indices (v0, v1, v2), (v3, v4, v5).
     */
    static void getDecomposedRectangleTriangleIndices(int* indices);

    /**
     * Decomposes a quadrilateral facet into two triangles and outputs the resulting vertex indices.
     * As there are two possible decompositions, it chooses the one which has the most coplanar triangles.
     * The output triangles' vertex indices are in counter-clockwise order.
     * @param[in] the facet vertices (4 (x,y,z) triplets, in counter-clockwise order).
     * @param[in] the facet vertices' indices (4 elements, in counter-clockwise order).
     * @param[out] the triangles' vertex indices (6 elements: two consecutive index triplets).
     */
    static void getDecomposedQuadTriangleIndices(double vertices[4][3], int* facetVertexIndices, int* triangleVertexIndices);
};

#endif
