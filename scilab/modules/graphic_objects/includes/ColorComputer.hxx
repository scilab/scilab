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

#ifndef COLORCOMPUTER_HXX
#define COLORCOMPUTER_HXX

#include  "DecompositionUtils.hxx"

/**
 * Offset passed to the getColor function for colors mapped to z values.
 */
#define Z_COLOR_OFFSET    0.5

/**
 * Offset passed to the getColor function for linearly mapped colors.
 */
#define COLOR_OFFSET      0.1

/**
 * Special color index values.
 */
enum SpecialColorIndexValues
{
    WHITE_LOWER_INDEX = -4,
    BLACK_LOWER_INDEX = -3,
    BLACK_UPPER_INDEX = 0
};

/**
 * The minimum value of a single R, G or B component.
 */
#define MIN_COMPONENT_VALUE    0.0

/**
 * The maximum value of a single R, G or B component.
 */
#define MAX_COMPONENT_VALUE    1.0

/**
 * The index value corresponding to white.
 */
#define WHITE_INDEX    -2.0

/**
 * The index value corresponding to black.
 */
#define BLACK_INDEX    -1.0

/**
 * An offset used to center the color index when computing
 * colormap texture coordinates.
 */
#define COLOR_TEXTURE_OFFSET    0.5

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
     * The output color is looked up in an RGB colormap, using a linear mapping between the latter and s.
     * It does not currently check whether srange is greater than 0.
     * If s is a Nan value, the black color is output.
     * @param[in] the scalar value.
     * @param[in] the interval's minimum value.
     * @param[in] the interval's range (smax-smin).
     * @param[in] an offset added to the index computed from s.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     */
    static void getColor(double s, double smin, double srange, double indexOffset, double* colormap, int colormapSize, float* returnedColor);

    /**
     * Outputs an RGB color mapped to a scalar value s belonging to an [smin, smax] interval.
     * The output color is looked up in an RGB colormap, using a linear mapping between a sub-interval of the latter and s.
     * It neither checks whether srange is greater than 0 nor verifies that the sub-interval is included within the colormap's
     * range (which must be ensured when specifying the sub-interval's bounds).
     * If s is a Nan value, the black color is output.
     * @param[in] the scalar value.
     * @param[in] the interval's minimum value.
     * @param[in] the interval's range (smax-smin).
     * @param[in] an offset added to the index computed from s.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[in] the index specifying the colormap sub-interval's lower bound.
     * @param[in] the index specifying the colormap sub-interval's upper bound.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     */
    static void getColor(double s, double smin, double srange, double indexOffset, double* colormap, int minIndex, int maxIndex,
                         int colormapSize, float* returnedColor);

    /**
     * Outputs an RGB color directly mapped to a scalar value s.
     * The output color is looked up in an RGB colormap, using s as a direct index.
     * White and black are respectively output when s <= -3 and -3 < s < 0 ; s is also
     * clamped to the colormap's upper bound (colormapSize-1). If s is a Nan value, black is also
     * output.
     * @param[in] the scalar value used as an index.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     * @param[in] a boolean to indicate if the color is clamped (by default true).
     */
    template <typename T>
    inline static void getDirectColor(T s, double* colormap, int colormapSize, float* returnedColor, bool clamped = true)
    {
        int index = 0;

        if (s <= (T) BLACK_LOWER_INDEX)
        {
            /* Clamp to white */
            returnedColor[0] = MAX_COMPONENT_VALUE;
            returnedColor[1] = MAX_COMPONENT_VALUE;
            returnedColor[2] = MAX_COMPONENT_VALUE;
        }
        else if ((((T) BLACK_LOWER_INDEX < s) && (s < (T) BLACK_UPPER_INDEX)) || !DecompositionUtils::isANumber(s))
        {
            /* Black is also output for Nan values */
            returnedColor[0] = MIN_COMPONENT_VALUE;
            returnedColor[1] = MIN_COMPONENT_VALUE;
            returnedColor[2] = MIN_COMPONENT_VALUE;
        }
        else
        {
            if (s > (T)(colormapSize - 1))
            {
                if (clamped)
                {
                    s = (T) (colormapSize - 1);
                }
                else
                {
                    returnedColor[0] = -1;
                    return;
                }
            }

            index = (int) s;

            returnedColor[0] = (float)colormap[index];
            returnedColor[1] = (float)colormap[colormapSize + index];
            returnedColor[2] = (float)colormap[2 * colormapSize + index];
        }
    }

    /**
     * Outputs an RGB color directly mapped to a scalar value s.
     * The output color is looked up in an RGB colormap, using s as a direct index.
     * White and black are respectively output when s <= -3 and -3 < s < 0 ; s is also
     * clamped to the colormap's upper bound (colormapSize-1). If s is a Nan value, black is also
     * output.
     * @param[in] the scalar value used as an index.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     * @param[in] a boolean to indicate if the color is clamped (by default true).
     */
    template <typename T>
    inline static void getDirectByteColor(T s, double* colormap, int colormapSize, unsigned char* returnedColor, bool clamped = true)
    {
        float color[3];
        unsigned int * rc = (unsigned int *)returnedColor;
        getDirectColor(s, colormap, colormapSize, color, clamped);

        if (!clamped && color[0] == -1)
        {
            *rc = 0;
        }
        else
        {
            returnedColor[0] = (unsigned char)(color[0] * 255);
            returnedColor[1] = (unsigned char)(color[1] * 255);
            returnedColor[2] = (unsigned char)(color[2] * 255);
            returnedColor[3] = 0xFF;
        }
    }

    /**
     * Returns a colormap index from a scalar value s.
     * Indices corresponding to white and black are respectively output when -4 < s <= -3 and -3 < s < 0;
     * if s <= -4, the lower colormap index is returned (0).
     * s is also clamped to the colormap's uppderbound (colormapSize-1).
     * @param[in] the scalar value used as an index.
     * @param[in] the colormap's size.
     * @return the index obtained from s.
     */
    static double getDirectIndex(double s, int colormapSize);

    /**
     * Outputs an RGB color directly mapped to a scalar value s.
     * The output color is looked up in an RGB colormap, using s as a direct index,
     * which is clamped to the colormap bounds, that is [0, colormapSize-1] . If s is a Nan value,
     * the black color is output.
     * @param[in] the scalar value used as an index.
     * @param[in] a pointer to the colormap used.
     * @param[in] the colormap's size.
     * @param[out] a pointer to the array into which the resulting color is output (its R, G, B components are written consecutively).
     */
    static void getClampedDirectColor(double s, double* colormap, int colormapSize, float* returnedColor);

    /**
     * Returns the given index rounded down and clamped to the colormap size.
     * @param[in] the scalar value used as an index.
     * @param[in] the colormap's size.
     * @return the index clamped to the colormap size.
     */
    static double getClampedDirectIndex(double s, int colormapSize);

    /**
     * Returns a colormap index mapped to a scalar value s belonging to an [smin, smax] interval.
     * The index belongs to an [imin, imax] interval which is linearly mapped to s, smin and smax respectively
     * corresponding to the imin and imax indices. The computed index is rounded down and then clamped to
     * the [imin, imax] interval.
     * It neither checks whether srange is greater than 0 nor verifies than minIndex and maxIndex are valid colormap
     * indices.
     * @param[in] the scalar value.
     * @param[in] the scale value's minimum value.
     * @param[in] the scalar value interval's range (smax-smin).
     * @param[in] an offset added to the index computed from s.
     * @param[in] the index specifying the index interval's lower bound.
     * @param[in] the index specifying the index interval's upper bound.
     * @return the computed index.
     */
    static double getIndex(double s, double smin, double srange, double indexOffset, int minIndex, int maxIndex);
};

#endif

