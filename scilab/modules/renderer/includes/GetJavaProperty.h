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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GET_JAVA_PROPERTY_H_
#define _GET_JAVA_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_renderer.h"
#include "ObjectStructure.h"
#include "BOOL.h"

/**
 * Get the colormap of a figure. The colormap is stored in java
 * so we need to access it.
 * @param pFigure figure from which we want the colormap
 * @param rgbMat matrix of size nbColor x 3 conating the 3 channels
 *               RGB of each color.
 */
RENDERER_IMPEXP void sciGetJavaColormap( sciPointObj * pFigure, double rgbMat[] ) ;

/**
 * Get the size of a figure object
 */
RENDERER_IMPEXP void sciGetJavaFigureSize( sciPointObj * pFigure, int size[2] ) ;

/**
 * Get the size of a window enclosing a figure
 */
RENDERER_IMPEXP void sciGetJavaWindowSize( sciPointObj * pFigure, int size[2] ) ;

/**
 * Set the position in pixels of a window enclosing a figure.
 */
RENDERER_IMPEXP void sciGetJavaWindowPosition( sciPointObj * pFigure, int pos[2] ) ;

/**
 * Get the info message of the figure.
 * @param pFigure figure from which we want the infoMessage
 * @param[out] infoMessage string filled with the message. Should have a minimum size
 *                         given by getInfoMessageLength.
 */
RENDERER_IMPEXP void sciGetJavaInfoMessage(sciPointObj * pFigure, char * infoMessage);

/**
 * @return size of the info message to be used in combination with getInfoMessage.
 */
RENDERER_IMPEXP int sciGetJavaInfoMessageLength(sciPointObj * pFigure);

/**
 * @return the number of pass used for antialiasing or 0 if antialiasing is disable.
 */
RENDERER_IMPEXP int sciGetJavaAntialiasingQuality(sciPointObj * pFigure);

/**
 * Convert user Coordinate to pixel coordinates.
 */
RENDERER_IMPEXP void sciGetJavaPixelCoordinates(sciPointObj * pSubwin, const double userCoord[3], int pixCoord[2]);

/**
 * Convert user Coordinate to pixel coordinates using 2d view.
 */
RENDERER_IMPEXP void sciGetJava2dViewPixelCoordinates(sciPointObj * pSubwin, const double userCoord[3], int pixCoord[2]);

/**
 * Project 3d coordinates on the Default 2d plane
 */
RENDERER_IMPEXP void sciGetJava2dViewCoordinates(sciPointObj * pSubwin, const double userCoords3D[3], double userCoords2D[2]);

/**
 * Convert pixel coordinates to 2D view ones.
 */
RENDERER_IMPEXP void sciGetJava2dViewCoordFromPixel(sciPointObj * pSubwin, const int pixCoords[2], double userCoords2D[2]);

/**
 * Get the bounding box of a text object.
 */
RENDERER_IMPEXP void sciGetJavaBoundingBox(sciPointObj * pText, double corner1[3], double corner2[3],
                           double corner3[3], double corner4[3]);

/**
 * Get the bounding box in pixels of a text object.
 */
RENDERER_IMPEXP void sciGetJavaPixelBoundingBox(sciPointObj * pText, int corner1[2], int corner2[2],
                                int corner3[2], int corner4[2]);

/**
 * Get the viewing area in pixels of a subwin object
 */
RENDERER_IMPEXP void sciGetJavaViewingArea(sciPointObj * pSubwin, int * xPos, int * yPos,
                           int * width, int * height);

/**
 * Get the bounding box used by a segs or champ object
 */
RENDERER_IMPEXP void sciGetJavaSegsBoundingBox(sciPointObj * pSegs, double bounds[6]);

/**
 * Get the auto resize status od a figure object
 */
RENDERER_IMPEXP BOOL sciGetJavaAutoResizeMode(sciPointObj * pFigure);

/**
 * Get the viewport on the canvas
 * @param viewport [x,y,w,h] array
 */
RENDERER_IMPEXP void sciGetJavaViewport(sciPointObj * pFigure, int viewport[4]);

/**
 * Get number of subticks along each axis of a subwindow.
 */
RENDERER_IMPEXP void sciGetJavaNbSubticks(sciPointObj * pSubwin, double subtics[3]);

/**
 * Get number of ticks along X axis of a subwindow.
 */
RENDERER_IMPEXP int sciGetJavaNbXTicks(sciPointObj * pSubwin);

/**
 * Get the labels and positions of ticks along X axis.
 * String composing ticksLabels are allocated with MALLOC.
 */
RENDERER_IMPEXP void sciGetJavaXTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);

/**
 * Get number of ticks along Y axis of a subwindow.
 */
RENDERER_IMPEXP int sciGetJavaNbYTicks(sciPointObj * pSubwin);

/**
 * Get the labels and positions of ticks along Y axis.
 * String composing ticksLabels are allocated with MALLOC.
 */
RENDERER_IMPEXP void sciGetJavaYTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);

/**
 * Get number of ticks along Z axis of a subwindow.
 */
RENDERER_IMPEXP int sciGetJavaNbZTicks(sciPointObj * pSubwin);

/**
 * Get the labels and positions of ticks along Z axis.
 * String composing ticksLabels are allocated with MALLOC.
 */
RENDERER_IMPEXP void sciGetJavaZTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);

/**
 * Check wether it is possible to create a graphic window or not
 */
RENDERER_IMPEXP BOOL sciGetJavaIsAbleToCreateWindow(void);


#ifdef __cplusplus
}
#endif

#endif /* _GET_JAVA_PROPERTY_H_ */
