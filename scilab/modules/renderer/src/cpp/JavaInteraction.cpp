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
extern "C" {
#include "JavaInteraction.h"
#include "getScilabJavaVM.h"
}

#include "CallRenderer.hxx"

void startInteractiveZoom(char *pstObjUID)
{
    org_scilab_modules_renderer::CallRenderer::startInteractiveZoom(getScilabJavaVM(), pstObjUID);
}

/*---------------------------------------------------------------------------------*/
void javaRubberBox(sciPointObj * pFigure, BOOL isClick, const int initialRect[4], int endRect[4], int * usedButton)
{
    //getFigureDrawer(pFigure)->rubberBox((isClick == TRUE), initialRect, endRect, usedButton);
}
/*---------------------------------------------------------------------------------*/
void interactiveJavaZoom(sciPointObj * pFigure)
{
    //getFigureDrawer(pFigure)->interactiveZoom();
}
/*---------------------------------------------------------------------------------*/
void interactiveJavaSubwinZoom(sciPointObj * pSubwin)
{
    //getSubwinDrawer(pSubwin)->interactiveZoom();
}
/*---------------------------------------------------------------------------------*/
void interactiveJavaRotation(sciPointObj * pFigure)
{
    //getFigureDrawer(pFigure)->interactiveRotation();
}
/*---------------------------------------------------------------------------------*/
void interactiveJavaSubwinRotation(sciPointObj * pSubwin)
{
    //getSubwinDrawer(pSubwin)->interactiveRotation();
}
/*---------------------------------------------------------------------------------*/
void javaUnzoomSubwin(sciPointObj * pSubwin)
{
    //getSubwinDrawer(pSubwin)->unzoom();
}
/*---------------------------------------------------------------------------------*/
void javaShowWindow(sciPointObj * pFigure)
{
    //getFigureDrawer(pFigure)->showWindow();
}
/*---------------------------------------------------------------------------------*/
