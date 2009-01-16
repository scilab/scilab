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
#include "BOOL.h"

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
 * @return indicates if the size could be successfully modified
 */
int sciSetJavaFigureSize( sciPointObj * pFigure, const int size[2] ) ;

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
 * Zoom a subwin object with the specified rectangle in pixels
 * @return TRUE if the axes box has been zoomed, FALSE otherwise
 */
BOOL sciJavaZoomRect(sciPointObj * pSubwin, int posX, int posY, int width, int height);

/**
 * Register a text object in order to it after any other object later.
 */
void sciJavaAddTextToDraw(sciPointObj * pText, sciPointObj * parentSubwin);

/**
 * Remove a text object in order to it after any other object later.
 */
void sciJavaRemoveTextToDraw(sciPointObj * pText, sciPointObj * parentSubwin);

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
 * Force a window to be be single buffered
 */
void sciSetJavaUseSingleBuffer(sciPointObj * pFigure, BOOL useSingleBuffer);

/**
 * Set the title of a figure.
 */
void sciSetJavaTitle( sciPointObj * pFigure, const char * title ) ;

/**
 * Update subwin size and scale
 */
void sciJavaUpdateSubwinScale(sciPointObj * pSubwin);

/**
 * Update bounding box of a text objectr if needed
 */
void sciJavaUpdateTextBoundingBox(sciPointObj * pText);

#ifdef __cplusplus
}
#endif

#endif /* _SET_JAVA_PROPERTY_H_ */
