/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Pedro SOUZA
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

#ifndef NORMAL_GENERATOR_HXX
#define NORMAL_GENERATOR_HXX

/**
 * Returns the specified quad's vetex normal.
 * (i,j) specifies which quad in the grid
 * and k specifies which quad's vertex (0 .. 3).
 */
float * getGridNormal(float *buffer, int numX, int numY, int elementSize, int i, int j, int k);

/**
 * Calculates the normal vector for each quad
 * averaging the normals of the 2 triangles that
 * compose the quad.
 */
bool CalculateGridNormalFlat(float* position, float* buffer, int bufferLength, int elementSize);

/**
 * Calculates the normal vector for each quad
 * in per-vertex mode averaging the normals of
 * shared vertices.
 */
bool CalculateGridNormalSmooth(float* position, float* buffer, int bufferLength, int elementSize, int numX, int numY);

/**
 * Calculates the normal vector for each given polygon.
 * All vertex normals from a given polygon are the
 * average sum of each normal of the decomposition
 * of that polygon in triangles.
 */
bool CalculatePolygonNormalFlat(float* position, float* buffer, int bufferLength, int elementSize, int polygonVertices);

#endif
