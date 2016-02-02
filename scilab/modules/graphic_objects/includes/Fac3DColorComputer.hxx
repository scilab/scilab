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

#ifndef FAC3DCOLORCOMPUTER_HXX
#define FAC3DCOLORCOMPUTER_HXX

#include <string>

extern "C" {
#include <stdlib.h>
}

/**
 * Fac3D color computer class
 * Outputs color values depending on the corresponding Fac3D objects's color
 * values and properties (color_flag, data_mapping, etc.).
 *
 * To do:
 * -extend to take into account constant or z-mapped colors (respectively color_flag == 0
 *  and color_flag == 1).
 */

class Fac3DColorComputer
{
private :
    /* To do: rename colors to values in order to take into account Z values */

    /** The Fac3D object's color values array */
    double* colors;

    /**
     * The Fac3D object's total number of color values
     * Must be equal either to numGons (per-facet colors) or
     * to numGons*numVerticesPerGon (per-vertex colors).
     */
    int numColors;

    /** The Fac3D object's color flag value */
    int colorFlag;

    /** A flag specifying whether color values are directly mapped (1) or scaled (0) */
    int dataMapping;

    /** The number of n-gons */
    int numGons;

    /** The number of vertices per n-gon */
    int numVerticesPerGon;

    /**
     * The number of colors per facet
     * Must be equal either to numVerticesPerGon (per-vertex colors) or 1 (per-facet colors).
     */
    int numColorsPerFacet;

    /** A flag specifying whether colors are defined per-facet (0) or per-vertex (1) */
    int perVertex;

    /** The maximum color value */
    double minColorValue;

    /** The minimum color value */
    double maxColorValue;

    /** The color range actually used (used maximum - used minimum) */
    double colorRange;

    /** The actual minimum value used */
    double usedMinColorValue;

    /* A flag specifying whether the color range's min and max bounds are different (1) or not (0) */
    int colorRangeValid;

public :

    /**
     * Fac3DColorComputer default constructor.
     * Initializes the value array to NULL and all other values to 0
     * (except dataMapping, set to 1).
     */
    Fac3DColorComputer(void);

    /**
     * Fac3DColorComputer constructor.
     * @param[in] the color array.
     * @param[in] the number of colors.
     * @param[in] the color flag.
     * @param[in] a flag specifying whether colors are direct indices into the colormap (1) or must be scaled (0).
     * @param[in] the number of n-gons.
     * @param[in] the number of vertices per n-gon.
     */
    Fac3DColorComputer(double* colors, int numColors, int colorFlag, int dataMapping, int numGons, int numVerticesPerGon);

    /**
     * Fac3DColorComputer destructor.
     */
    ~Fac3DColorComputer();

    /**
     * Indicates whether facets are shaded with a single color or using interpolated shading.
     * Interpolated shading is used only if colors are defined per vertex and color flag is equal to 3.
     * Single color (flat) shading is used in any other case.
     * @return 1 if interpolated shading is used, 0 if flat shading is used.
     */
    int isInterpolatedShadingUsed(void);

    /**
     * Returns the color of facet i at vertex j which is actually output for rendering,
     * where 0 <= j < numVerticesPerGon.
     * The colors actually output for rendering may differ from the facet's color values for scaled
     * colors if the color range is invalid, that is, if the min and max color
     * values are equal. In this case, it returns the color at half the actually used color range.
     * In any other case, it returns the facet's color values by calling getFacetColor.
     * @param[in] the facet index.
     * @param[in] the vertex index j.
     * @return the facet color at vertex j.
     */
    double getOutputFacetColor(int facetIndex, int vertexIndex);

    /**
     * Returns the color of facet i at vertex j,
     * where 0 <= j < numVerticesPerGon.
     * The vertex index j is relevant only when per-vertex colors are specified and
     * color_flag is equal to 3. In any other case, a given facet's color
     * is the same for all its vertices, j is therefore ignored.
     * @param[in] the facet index.
     * @param[in] the vertex index j.
     * @return the facet color at vertex j.
     */
    double getFacetColor(int facetIndex, int vertexIndex);

    /**
     * Compute a facet's average color value.
     * @param[in] the facet index.
     * @return the facet's average color value.
     */
    double computeAverageValue(int facetIndex);

    /**
     * Determines whether a facet's color values are valid or not.
     * @param[in] the facet index.
     * @return 1 if the facet's color values are valid, 0 otherwise.
     */
    int isFacetColorValid(int facetIndex);

    /**
     * Computes the minimum and maximum color values,
     * which are then stored as members.
     */
    void computeMinMaxValues(void);

    /**
     * Computes the minimum and maximum values of a facet.
     * The input minimum and maximum values are updated to the facet's ones if the latter
     * are respectively less than and greater than the former.
     * @param[in] the facet index.
     * @param[in,out] the current minimum value, updated to the facet's minimum.
     * @param[in,out] the current maximum value, updated to the facet's maximum.
     */
    void computeFacetMinMax(int facetIndex, double* currentMin, double* currentMax);

    /**
     * Returns the minimum color value and the color range which are
     * actually used to get scaled colors.
     * The maximum color value is equal to the minimum one added to the color range.
     * @param[out] the minimum color value.
     * @param[out] the color range.
     */
    void getColorRangeValue(double* minColorValue, double* colorRange);
};

#endif
