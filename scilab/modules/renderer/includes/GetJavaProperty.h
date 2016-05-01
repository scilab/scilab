/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * desc : Interface functions between between GetProperty functions and
 * the C++/Java part of module
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

#ifndef _GET_JAVA_PROPERTY_H_
#define _GET_JAVA_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_renderer.h"

/**
 * Convert user Coordinate to pixel coordinates using 2d view.
 */
RENDERER_IMPEXP void sciGetJava2dViewPixelCoordinates(int iSubwinUID, const double userCoords[3], int pixCoords[2]);

/**
 * Project 3d coordinates on the Default 2d plane
 */
RENDERER_IMPEXP void sciGetJava2dViewCoordinates(int iSubwinUID, const double userCoords3D[3], double userCoords2D[2]);

/**
 * Convert pixel coordinates to 2D view ones.
 */
RENDERER_IMPEXP void sciGetJava2dViewCoordFromPixel(int iSubwinUID, const int pixCoords[2], double userCoords2D[2]);

/**
 * Get the viewing area in pixels of a subwin object
 */
RENDERER_IMPEXP void sciGetJavaViewingArea(int iSubwinUID, int * xPos, int * yPos,
        int * width, int * height);

#ifdef __cplusplus
}
#endif

#endif /* _GET_JAVA_PROPERTY_H_ */
