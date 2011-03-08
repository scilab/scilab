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

#ifndef DECOMPOSITION_UTILS_H
#define DECOMPOSITION_UTILS_H

/**
 * Decomposition utility class
 * A set of various static utility methods.
 */

class DecompositionUtils
{

public :

    /**
     * Tests whether a point is valid.
     * @param[in] the point's x-coordinate.
     * @param[in] the point's y-coordinate.
     * @param[in] the point's z-coordinate.
     * @return 1 if the point is valid, 0 if it is not.
     */
    static int isValid(double x, double y, double z);

    /**
     * Returns the base-10 logarithm of the input value.
     * @param[in] the input value.
     * @return the base-10 logarithm of the input value. 
     */
    static double getLog10Value(double value);

    /**
     * Determines if a point is valid in logarithmic scale.
     * It checks whether any of the component is strictly less than 0 (if the
     * logarithmic scale applies). In the event of the former, the point is
     * determined as not valid.
     * @param[in] the point's x-coordinate.
     * @param[in] the point's y-coordinate.
     * @param[in] the point's z-coordinate.
     * @param[in] the bit mask specifying for which coordinates the logarithmic scale is used.
     * @return 1 if the point is valid, 0 if it is not.
     */
    static int isLogValid(double x, double y, double z, int logMask);
};

#endif
