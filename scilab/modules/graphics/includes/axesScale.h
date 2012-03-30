/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: axesScale.h                                                      */
/* desc : Contains functions to compute scale changes in a specific       */
/*        subwindow.                                                      */
/*------------------------------------------------------------------------*/

#ifndef _AXES_SCALE_H_
#define _AXES_SCALE_H_
#include "dynlib_graphics.h"
#include "ObjectStructure.h"

/*------------------------------------------------------------------------------*/
GRAPHICS_IMPEXP int sciZoom2D(char * subwinUID, const double zoomRect[4]);
GRAPHICS_IMPEXP int sciZoom3D(char * subwinUID, const double zoomBox[6]);
GRAPHICS_IMPEXP int sciZoomRect(sciPointObj * pObj, const double zoomRect[4]);
GRAPHICS_IMPEXP int sciDefaultZoom2D(const double zoomRect[4]);
GRAPHICS_IMPEXP int sciFigureZoom2D(sciPointObj * figure, const double zoomRect[4]);
GRAPHICS_IMPEXP void sciGetZoom3D(sciPointObj * pObj, double zoomBox[6]);
GRAPHICS_IMPEXP void sciDefaultInteractiveZoom(void);
GRAPHICS_IMPEXP void sciInteractiveZoom(sciPointObj * pObj);
GRAPHICS_IMPEXP void sciUnzoomSubwin(char* subwinUID);
GRAPHICS_IMPEXP void sciUnzoomFigure(char* figureUID);
GRAPHICS_IMPEXP void sciUnzoomArray(char* objectsUID[], int nbSubwin);

/**
 * Update subwin size and scale if needed
 */
GRAPHICS_IMPEXP void updateSubwinScale(char * pSubwinUID);

/**
 * Update text bounds if needed
 */
GRAPHICS_IMPEXP void updateTextBounds(char * pTextUID);

/**
 * Zoom an object using a rectangular selection given in pixels.
 * @param pObj must be a figure or subwin handle
 * @param x1 X coordinate of a corner of the selection
 * @param y1 Y coordinate of a corner of the selection
 * @param x2 X coordinate of the oppoiste corner of the selection
 * @param y2 Y coordinate of the opposite corner of the selection
 */
GRAPHICS_IMPEXP void sciZoomObject(sciPointObj * pObj, int x1, int y1, int x2, int y2);

/**
 * Unzoom a subwindow
 */
GRAPHICS_IMPEXP void unzoomSubwin(sciPointObj * pSubwin);
/*------------------------------------------------------------------------------*/
GRAPHICS_IMPEXP BOOL checkDataBounds(char * subwinUID, double xMin, double xMax,
                     double yMin, double yMax, double zMin, double zMax);
/*------------------------------------------------------------------------------*/


#endif /* _AXES_SCALE_H_ */
