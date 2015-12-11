/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Pedro SOUZA
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
