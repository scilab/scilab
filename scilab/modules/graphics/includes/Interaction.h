/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library 
 *    Comment:
 *    This file contains all functions used to INTERACT with the graphic
 *    window (zoom by pressing button, callbacks...)
 --------------------------------------------------------------------------*/

#include "ObjectStructure.h"
#include "HandleManagement.h"

#ifndef __SCI_INTERACTION__
#define __SCI_INTERACTION__

/**
 * Different status after resizing a canvas or a window.
 */
typedef enum
{
  RESIZE_SUCCESS = 0,
  RESIZE_MULTIPLE_DOCKED_TAB = -1,
  RESIZE_SIZE_TOO_LARGE = -2,
  RESIZE_UNCHANGED = 1
} EResizeStatus;



extern int sciAddCallback (sciPointObj * pthis, char *code, int len, int mevent); /* INTERACTION */
extern int sciExecCallback (sciPointObj * pthis); /* INTERACTION */
extern int sciGetCallbackMouseEvent (sciPointObj * pthis); /* INTERACTION */
extern int sciSetCallbackMouseEvent (sciPointObj * pthis, int mevent); /* INTERACTION */
extern int sciGetCallbackLen (sciPointObj * pthis); /* INTERACTION */
extern char *sciGetCallback (sciPointObj * pthis); /* INTERACTION */
extern int sciDelCallback (sciPointObj * pthis); /* INTERACTION */

int Objmove(sciPointObj * pobj, double d[], int m, BOOL opt); /* INTERACTION */
extern BOOL sciIsAreaZoom(integer *box, integer *box1 ,integer *section); /* INTERACTION */

/**
 * Call rubber box on a figure in pixels
 * @param pFigure figure on which to apply the rubber box
 * @param isClick specify whether the rubber box is selected by mouse click
 *        or a sequence of press/release.
 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
 * @param intialRect if not null, specify the initial rectangle to use
 * @param[out] endRect array containing the coordinates of two opposite corners of
 *                     the rubber box
 * @param[out] usedButton Scilab code of the button used to terminate the rubber box
 */
void pixelRubberBox(sciPointObj * pFigure, BOOL isClick, BOOL isZoom,
                    const int initialRect[4], int endRect[4], int * usedButton);

/**
 * Call rubber box using a subwn scale
 * @param pFigure figure on which to apply the rubber box
 * @param isClick specify whether the rubber box is selected by mouse click
 *        or a sequence of press/release.
 * @param isZoom specify if the rubber box is used for a zoom and then change the mouse cursor.
 * 
 * @param intialRect if not null, specify the initial rectangle [x,y,w,h] to use
 * @param[out] endRect array containing the [x,y,w,h] of the selected rectangle
 * @param[out] usedButton Scilab code of the button used to terminate the rubber box
 */
void rubberBox(sciPointObj * pSubwin, BOOL isClick, BOOL isZoom,
               const double initialRect[4], double endRect[4], int * usedButton);


/**
 * Interactive rotation of a subwin within the figure.
 */
void interactiveRotation(sciPointObj * pFigure);

/**
 * Interactive rotation of a subwindow
 */
void interactiveSubwinRotation(sciPointObj * pSubiwn);

/**
 * Put the figure in top of other windows.
 * @pram pFigure figure corresponding to the window
 */
void showWindow(sciPointObj * pFigure);


#endif /* __SCI_INTERACTION__ */
