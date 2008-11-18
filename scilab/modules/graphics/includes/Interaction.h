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

#include "BOOL.h"

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
extern int sciGetCallbackMouseEvent (sciPointObj * pthis); /* INTERACTION */
extern int sciSetCallbackMouseEvent (sciPointObj * pthis, int mevent); /* INTERACTION */
extern char *sciGetCallback (sciPointObj * pthis); /* INTERACTION */
extern int sciDelCallback (sciPointObj * pthis); /* INTERACTION */

int Objmove(sciPointObj * pobj, double d[], int m, BOOL opt); /* INTERACTION */

/**
 * Call rubber box on a figure in pixels
 * @param pFigure figure on which to apply the rubber box
 * @param isClick specify whether the rubber box is selected by mouse click
 *        or a sequence of press/release.
 * @param intialRect if not null, specify the initial rectangle to use
 * @param[out] endRect array containing the coordinates of two opposite corners of
 *                     the rubber box
 * @param[out] usedButton Scilab code of the button used to terminate the rubber box
 */
void pixelRubberBox(sciPointObj * pFigure, BOOL isClick,
                    const int initialRect[4], int endRect[4], int * usedButton);

/**
 * Call rubber box using a subwn scale
 * @param pFigure figure on which to apply the rubber box
 * @param isClick specify whether the rubber box is selected by mouse click
 *        or a sequence of press/release.
 * @param intialRect if not null, specify the initial rectangle [x,y,w,h] to use
 * @param[out] endRect array containing the [x,y,w,h] of the selected rectangle
 * @param[out] usedButton Scilab code of the button used to terminate the rubber box
 */
void rubberBox(sciPointObj * pSubwin, BOOL isClick,
               const double initialRect[4], double endRect[4], int * usedButton);


/**
 * Perform an interative zoom on a subwin or figure handle;
 */
void interactiveZoom(sciPointObj * pObj);

/**
 * Interactive rotation of a subwin within the figure.
 */
void interactiveRotation(sciPointObj * pFigure);

/**
 * Interactive rotation of a subwindow
 */
void interactiveSubwinRotation(sciPointObj * pSubwin);

/**
 * Put the figure in top of other windows.
 * @pram pFigure figure corresponding to the window
 */
void showWindow(sciPointObj * pFigure);

/**
 * Modify a subwin viewing angles by a certain amount.
 */
void updateViewingAngles(sciPointObj * pSubwin, double deltaAlpha, double deltaTheta);


#endif /* __SCI_INTERACTION__ */
