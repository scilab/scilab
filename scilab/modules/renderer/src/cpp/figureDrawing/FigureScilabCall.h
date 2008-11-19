/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Call from Java to figure drawing code using JNI
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _FIGURE_SCILAB_CALL_
#define _FIGURE_SCILAB_CALL_

#ifdef __cplusplus
extern "C" {
#endif

void displayFigure(int figureId);

void redrawFigure(int figureId) ;

void redrawSubwins(int figureId) ;

void rotateSubwin(long long subwinHandle, double deltaAlpha, double deltaTheta);

long long getClickedSubwinHandle(int figureId, int clickXCoord, int clickYCoord);

void zoomObject(long long objectHandle, int x1, int y1, int x2, int y2);

void unzoomSubwinHandle(long long subwinHandle);

#ifdef __cplusplus
}
#endif
#endif /* _FIGURE_SCILAB_CALL_ */
