/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * desc : Interface functions between between SetProperty functions and the C++/Java part of module
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SetJavaProperty.h"
#include "getHandleDrawer.h"
#include "figureDrawing/DrawableFigure.h"
#include "subwinDrawing/DrawableSubwin.h"

using namespace sciGraphics ;

/*---------------------------------------------------------------------------------*/
void sciSetJavaColormap( sciPointObj * pFigure, const double rgbMat[], int nbColor )
{
  getFigureDrawer(pFigure)->setColorMap( rgbMat, nbColor ) ;
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaFigureSize( sciPointObj * pFigure, const int size[2] )
{
  getFigureDrawer(pFigure)->setSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaWindowSize( sciPointObj * pFigure, const int size[2] )
{
  getFigureDrawer(pFigure)->setWindowSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaWindowPosition( sciPointObj * pFigure, const int pos[2] )
{
  getFigureDrawer(pFigure)->setWindowPosition(pos) ;
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaInfoMessage( sciPointObj * pFigure, const char * infoMessage )
{
  getFigureDrawer(pFigure)->setInfoMessage(infoMessage);
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaPixmapMode(sciPointObj * pFigure, BOOL onOrOff)
{
  getFigureDrawer(pFigure)->setPixmapMode(onOrOff == TRUE);
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaRenderingEnable(sciPointObj * pFigure, BOOL isEnable)
{
  getFigureDrawer(pFigure)->setRenderingEnable(isEnable == TRUE);
}
/*---------------------------------------------------------------------------------*/
BOOL sciJavaZoomRect(sciPointObj * pSubwin, int posX, int posY, int width, int height)
{
  if (getSubwinDrawer(pSubwin)->getCamera()->zoomRect(posX, posY, width, height))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaAutoResizeMode(sciPointObj * pFigure, BOOL resizeMode)
{
  getFigureDrawer(pFigure)->setAutoResizeMode(resizeMode == TRUE);
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaViewport(sciPointObj * pFigure, const int viewport[4])
{
  getFigureDrawer(pFigure)->setViewport(viewport);
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaBackground(sciPointObj * pFigure, int backColor)
{
  getFigureDrawer(pFigure)->setBackgroundColor(backColor);
}
/*---------------------------------------------------------------------------------*/
void sciSetJavaTitle( sciPointObj * pFigure, const char * title )
{
  getFigureDrawer(pFigure)->setTitle(title);
}
/*---------------------------------------------------------------------------------*/
