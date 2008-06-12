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

#include "ObjectStructure.h"

/**
 * Call rubber box on a figure
 * @param pFigure figure on which to apply the rubber box
 * @param isClick specify whether the rubber box is selected by mouse click
 *        or a sequence of press/release.
 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
 * @param intialRect if not null, specify the initial rectangle to use
 * @param[out] endRect array containing the coordinates of two opposite corners of
 *                     the rubber box
 * @param[out] usedButton Scilab code of the button used to terminate the rubber box
 */
void javaRubberBox(sciPointObj * pFigure, BOOL isClick, BOOL isZoom, const int initialRect[4], int endRect[4], int * usedButton);

/**
 * Get current displacement in the graphic window, to be used for axes rotation.
 * @param pFigure figure corresponding to the window
 * @param displacement [dx, dy] is the mouse displacement in pixels
 *         or the position of the mouse with the first call.
 * @return FALSE if the displacement tracking has ended, TRUE otherwise.
 */
BOOL getJavaRotationDisplacement(sciPointObj * pFigure, int displacement[2]);

/**
 * If a rotation displacement is recording, cancel it.
 * @param pFigure figure corresponding to the window
 */
void stopJavaRotationRecording(sciPointObj * pFigure);

/**
 * Put the figure in top of other windows.
 * @pram pFigure figure corresponding to the window
 */
void javaShowWindow(sciPointObj * pFigure);

#ifdef __cplusplus
}
#endif

#endif /* _JAVA_INTERACTION_H_ */
