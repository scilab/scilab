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

#ifndef COLORCOMPUTER_H
#define COLORCOMPUTER_H

/**
 * ColorComputer class
 * Various utility functions used to compute colors.
 * To be extended.
 */
class ColorComputer
{

public :

    /**
     * Ouputs an RGB color mapped to a scalar value s belonging to an [smin, smax] interval.
     * The output color is looked up into an RGB colormap, using a linear mapping between the latter and s.
     * It does not currently check whether srange is greater than 0, nor that s belongs to the [smin, smax]
     * interval.
     * @param[in] the scalar value.
     * @param[in] the interval's minimum value.
     * @param[in] the interval's range (smin-smax).
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     */
    static void getColor(double s, double smin, double srange, double* colormap, int colormapSize, float* returnedColor);

};

#endif
