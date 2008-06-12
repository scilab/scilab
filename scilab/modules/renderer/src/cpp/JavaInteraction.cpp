/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "getHandleDrawer.h"
#include "figureDrawing/DrawableFigure.h"
#include "JavaInteraction.h"

using namespace sciGraphics ;

/*---------------------------------------------------------------------------------*/
void javaRubberBox(sciPointObj * pFigure, BOOL isClick, BOOL isZoom, const int initialRect[4], int endRect[4], int * usedButton)
{
  getFigureDrawer(pFigure)->rubberBox((isClick == TRUE), (isZoom == TRUE), initialRect, endRect, usedButton);
}
/*---------------------------------------------------------------------------------*/
BOOL getJavaRotationDisplacement(sciPointObj * pFigure, int displacement[2])
{
  if (getFigureDrawer(pFigure)->getRotationDisplacement(displacement))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/*---------------------------------------------------------------------------------*/
void stopJavaRotationRecording(sciPointObj * pFigure)
{
  getFigureDrawer(pFigure)->stopRotationRecording();
}
/*---------------------------------------------------------------------------------*/
void javaShowWindow(sciPointObj * pFigure)
{
  getFigureDrawer(pFigure)->showWindow();
}
/*---------------------------------------------------------------------------------*/
