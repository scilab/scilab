/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/**
 * @file JavaInteraction.h
 *
 * File regrouping routines used to interact with Java graphic objects
 */

#ifndef _JAVA_INTERACTION_H_
#define _JAVA_INTERACTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_renderer.h"
#include "ObjectStructure.h"
#include "BOOL.h"

/**
 * Call rubber box on a figure
 * @param pFigure figure on which to apply the rubber box
 * @param isClick specify whether the rubber box is selected by mouse click
 *        or a sequence of press/release.
 * @param intialRect if not null, specify the initial rectangle to use
 * @param[out] endRect array containing the coordinates of two opposite corners of
 *                     the rubber box
 * @param[out] usedButton Scilab code of the button used to terminate the rubber box
 */
RENDERER_IMPEXP void javaRubberBox(sciPointObj * pFigure, BOOL isClick, const int initialRect[4], int endRect[4], int * usedButton);

/**
 * Perform an interactive zoom of the figure using a rectangular selection.
 * @param pFigure figure to zoom.
 */
RENDERER_IMPEXP void interactiveJavaZoom(sciPointObj * pFigure);

/**
 * Perform an interactive zoom of the subwin using a rectangular selection.
 * @param pSubwin subwin to zoom.
 */
RENDERER_IMPEXP void interactiveJavaSubwinZoom(sciPointObj * pSubwin);

/**
 * Perform an interactive rotation of a subwin contained in the figure.
 * @param pFigure figure corresponding to the canvas
 */
RENDERER_IMPEXP void interactiveJavaRotation(sciPointObj * pFigure);

/**
 * Perform an interactive rotation of a subwin with the mouse.
 * @param pSubwin subwin to rotate
 */
RENDERER_IMPEXP void interactiveJavaSubwinRotation(sciPointObj * pSubwin);

/**
 * Unzoom a subwindow
 * @param pSubwin subwin to unzoom
 */
RENDERER_IMPEXP void javaUnzoomSubwin(sciPointObj * pSubwin);

/**
 * Put the figure in top of other windows.
 * @pram pFigure figure corresponding to the window
 */
RENDERER_IMPEXP void javaShowWindow(sciPointObj * pFigure);

#ifdef __cplusplus
}
#endif

#endif /* _JAVA_INTERACTION_H_ */
