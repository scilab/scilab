/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Interface functions between between SetProperty functions and
 * the C++/Java part of module
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SET_JAVA_PROPERTY_H_
#define _SET_JAVA_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ObjectStructure.h"

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
 * Specify if the figure can be renderered now
 */
void sciSetJavaRenderingEnable(sciPointObj * pFigure, BOOL isEnable);

/**
 * Zoom a subwin object with the specified rectangle in pixels
 * @return TRUE if the axes box has been zoomed, FALSE otherwise
 */
 BOOL sciJavaZoomRect(sciPointObj * pSubwin, int posX, int posY, int width, int height);

/**
 * Set the auto_resize mode of a figure
 */
void sciSetJavaAutoResizeMode(sciPointObj * pFigure, BOOL resizeMode);

/**
 * Set the position and size of the viewport on the canvas
 * @param viewport [x,y,w,h] array
 */
void sciSetJavaViewport(sciPointObj * pFigure, const int viewport[4]);

/**
 * Set the background color of canvas
 */
void sciSetJavaBackground(sciPointObj * pFigure, int backColor);

/**
 * Set the title of a figure.
 */
void sciSetJavaTitle( sciPointObj * pFigure, const char * title ) ;

#ifdef __cplusplus
}
#endif

#endif /* _SET_JAVA_PROPERTY_H_ */
