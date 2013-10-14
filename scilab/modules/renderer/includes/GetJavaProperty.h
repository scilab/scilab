/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * desc : Interface functions between between GetProperty functions and
 * the C++/Java part of module
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
