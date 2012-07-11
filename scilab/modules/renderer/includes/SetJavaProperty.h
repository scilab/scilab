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

#include "dynlib_renderer.h"
#include "ObjectStructure.h"
#include "BOOL.h"

/**
 * Zoom a subwin object with the specified rectangle in pixels
 * @return TRUE if the axes box has been zoomed, FALSE otherwise
 */
RENDERER_IMPEXP BOOL sciJavaZoomRect(sciPointObj * pSubwin, int posX, int posY, int width, int height);

/**
 * Register a text object in order to it after any other object later.
 */
RENDERER_IMPEXP void sciJavaAddTextToDraw(sciPointObj * pText, sciPointObj * parentSubwin);

/**
 * Remove a text object in order to it after any other object later.
 */
RENDERER_IMPEXP void sciJavaRemoveTextToDraw(sciPointObj * pText, sciPointObj * parentSubwin);

/**
 * Set the auto_resize mode of a figure
 */
RENDERER_IMPEXP void sciSetJavaAutoResizeMode(sciPointObj * pFigure, BOOL resizeMode);

/**
 * Set the position and size of the viewport on the canvas
 * @param viewport [x,y,w,h] array
 */
RENDERER_IMPEXP void sciSetJavaViewport(sciPointObj * pFigure, const int viewport[4]);

/**
 * Set the background color of canvas
 */
RENDERER_IMPEXP void sciSetJavaBackground(sciPointObj * pFigure, int backColor);

/**
 * Force a window to be be single buffered
 */
RENDERER_IMPEXP void sciSetJavaUseSingleBuffer(sciPointObj * pFigure, BOOL useSingleBuffer);

/**
 * Set the title of a figure.
 */
RENDERER_IMPEXP void sciSetJavaTitle( sciPointObj * pFigure, const char * title ) ;

/**
 * Update subwin size and scale
 */
RENDERER_IMPEXP void sciJavaUpdateSubwinScale(char * pSubwinUID);

/**
 * Update bounding box of a text object if needed
 */
RENDERER_IMPEXP void sciJavaUpdateTextBoundingBox(char * pTextUID);

#ifdef __cplusplus
}
#endif

#endif /* _SET_JAVA_PROPERTY_H_ */
