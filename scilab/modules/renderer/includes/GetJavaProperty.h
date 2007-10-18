/*------------------------------------------------------------------------*/
/* file: GetJavaProperty.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface functions between between GetProperty functions and   */
/*        the C++/Java part of module                                     */
/*------------------------------------------------------------------------*/

#ifndef _GET_JAVA_PROPERTY_H_
#define _GET_JAVA_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "../../../graphics/includes/ObjectStructure.h"

/**
 * Get the colormap of a figure. The colormap is stored in java
 * so we need to access it.
 * @param pFigure figure from which we want the colormap
 * @param rgbMat matrix of size nbColor x 3 conating the 3 channels
 *               RGB of each color.
 */
void sciGetJavaColormap( sciPointObj * pFigure, double rgbMat[] ) ;

/**
 * Get the size of a figure object
 */
void sciGetJavaFigureSize( sciPointObj * pFigure, int size[2] ) ;

/**
 * Get the size of a window enclosing a figure
 */
void sciGetJavaWindowSize( sciPointObj * pFigure, int size[2] ) ;

/**
 * Set the position in pixels of a window enclosing a figure.
 */
void sciGetJavaWindowPosition( sciPointObj * pFigure, int pos[2] ) ;

/**
 * Get the pixmap mode for figures.
 */
BOOL sciGetJavaPixmapMode(sciPointObj * pFigure);

/**
 * Convert user Coordinate to pixel coordinates.
 */
void sciGetJavaPixelCoordinates(sciPointObj * pSubwin, const double userCoord[3], int pixCoord[2]);

#ifdef __cplusplus
}
#endif

#endif /* _GET_JAVA_PROPERTY_H_ */
