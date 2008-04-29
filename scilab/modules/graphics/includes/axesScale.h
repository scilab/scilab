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

#include "ObjectStructure.h"

/*------------------------------------------------------------------------------*/
double InvAxis( double min, double max, double u ) ;
int ReverseDataFor3DXonly( sciPointObj * psubwin, double xvect[], int n1 ) ;
int ReverseDataFor3DYonly( sciPointObj * psubwin, double yvect[], int n1 ) ;
int ReverseDataFor3DZonly( sciPointObj * psubwin, double zvect[], int n1 ) ;
int ReverseDataFor3D(      sciPointObj * psubwin, double xvect[], double yvect[], double zvect[], int n1 ) ;
/*------------------------------------------------------------------------------*/
int trans3d( sciPointObj * pobj,
             integer       n   ,
             integer       xm[],
             integer       ym[],
             double        x[] ,
             double        y[] ,
             double        z[]  ) ;
/*------------------------------------------------------------------------------*/
int sciZoom2D(sciPointObj * subwin, const double zoomRect[4]);
int sciZoom3D(sciPointObj * subwin, const double zoomBox[6]);
int sciZoomRect(sciPointObj * pObj, const double zoomRect[4]);
int sciDefaultZoom2D(const double zoomRect[4]);
int sciFigureZoom2D(sciPointObj * figure, const double zoomRect[4]);
void sciGetZoom3D(sciPointObj * pObj, double zoomBox[6]);
void sciDefaultInteractiveZoom(void);
void sciInteractiveZoom(sciPointObj * pObj);
void sciUnzoomSubwin(sciPointObj * subwin);
void sciUnzoomFigure(sciPointObj * figure);
void sciUnzoomAll(void);
void sciUnzoomArray(sciPointObj * zoomedObjects[], int nbSubwin);

/**
 * Update subwin size and scale if needed
 */
void updateSubwinScale(sciPointObj * pSubwin);

/**
 * Update text bounds if needed
 */
void updateTextBounds(sciPointObj * pText);
/*------------------------------------------------------------------------------*/
BOOL checkDataBounds(sciPointObj * pObj, double xMin, double xMax,
                     double yMin, double yMax, double zMin, double zMax);
/*------------------------------------------------------------------------------*/


#endif /* _AXES_SCALE_H_ */
