/*------------------------------------------------------------------------*/
/* file: SetJavaProperty.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface functions between between SetProperty functions and   */
/*        the C++/Java part of module                                     */
/*------------------------------------------------------------------------*/

#ifndef _SET_JAVA_PROPERTY_H_
#define _SET_JAVA_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ObjectStructure.h"
#include "xs2file.h" /* graphic_export */

/**
 * Change the colormap of a figure. The colormap is stored in java
 * so we need to access it.
 * @param pFigure the figure we wants to modify
 * @param rgbMat matrix of size nbColor x 3 conating the 3 channels
 *               RGB of each color.
 * @param nbColor number of color in the new colormap
 */
void sciSetJavaColormap( sciPointObj * pFigure, const double rgbMat[], int nbColor ) ;

/**
 * Set the size of a figure object
 */
void sciSetJavaFigureSize( sciPointObj * pFigure, const int size[2] ) ;

/**
 * Set the size of a window enclosing a figure.
 */
void sciSetJavaWindowSize( sciPointObj * pFigure, const int size[2] ) ;

/**
 * Set the position in pixels of a window enclosing a figure.
 */
void sciSetJavaWindowPosition( sciPointObj * pFigure, const int pos[2] ) ;

/**
 * Set the message displayed by a figure.
 */
void sciSetJavaInfoMessage( sciPointObj * pFigure, const char * infoMessage ) ;

/**
 * Set the pixmap mode for figures.
 */
void sciSetJavaPixmapMode(sciPointObj * pFigure, BOOL onOrOff);

/**
 * Specify if the figure can be renderered now
 */
void sciSetJavaRenderingEnable(sciPointObj * pFigure, BOOL isEnable);

/**
 * Zoom a subwin object with the specified rectangle in pixels
 */
void sciJavaZoomRect(sciPointObj * pSubwin, int posX, int posY, int width, int height);

/**
 * Export a figure as a bitmap file
 */
void sciJavaExportToFile(sciPointObj * pFigure, const char * fileName, ExportFileType fileType);

#ifdef __cplusplus
}
#endif

#endif /* _SET_JAVA_PROPERTY_H_ */
