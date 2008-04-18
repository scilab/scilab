/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Interface functions between between GetProperty functions and the C++/Java part of module 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetJavaProperty.h"
#include "getHandleDrawer.h"
#include "figureDrawing/DrawableFigure.h"

using namespace sciGraphics ;

/*---------------------------------------------------------------------------------*/
void sciGetJavaColormap( sciPointObj * pFigure, double rgbMat[] )
{
  getFigureDrawer(pFigure)->getColorMap( rgbMat ) ;
}
/*---------------------------------------------------------------------------------*/
void sciGetJavaFigureSize( sciPointObj * pFigure, int size[2] )
{
  getFigureDrawer(pFigure)->getSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void sciGetJavaWindowSize( sciPointObj * pFigure, int size[2] )
{
  getFigureDrawer(pFigure)->getWindowSize(size) ;
}
/*---------------------------------------------------------------------------------*/
void sciGetJavaWindowPosition( sciPointObj * pFigure, int pos[2] )
{
  getFigureDrawer(pFigure)->getWindowPosition(pos) ;
}
/*---------------------------------------------------------------------------------*/
void sciGetJavaPixelCoordinates(sciPointObj * pSubwin, const double userCoord[3], int pixCoord[2])
{
  getSubwinDrawer(pSubwin)->getCamera()->getPixelCoordinates(userCoord, pixCoord);
}
/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewPixelCoordinates(sciPointObj * pSubwin, const double userCoord[3], int pixCoord[2])
{
   getSubwinDrawer(pSubwin)->getCamera()->get2dViewPixelCoordinates(userCoord, pixCoord);
}
/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewCoordinates(sciPointObj * pSubwin, const double userCoords3D[3], double userCoords2D[2])
{
  getSubwinDrawer(pSubwin)->getCamera()->get2dViewCoordinates(userCoords3D, userCoords2D);
}
/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewCoordFromPixel(sciPointObj * pSubwin, const int pixCoords[2], double userCoords2D[2])
{
  getSubwinDrawer(pSubwin)->getCamera()->get2dViewCoordinates(pixCoords, userCoords2D);
}
/*---------------------------------------------------------------------------------*/
void sciGetJavaPixelBoundingBox(sciPointObj * pText, int corner1[2], int corner2[2],
                                int corner3[2], int corner4[2])
{
  getTextDrawer(pText)->getScreenBoundingBox(corner1, corner2, corner3, corner4);
}
/*---------------------------------------------------------------------------------*/
void sciGetJavaViewingArea(sciPointObj * pSubwin, int * xPos, int * yPos,
                           int * width, int * height)
{
  getSubwinDrawer(pSubwin)->getCamera()->getViewingArea(xPos, yPos, width, height);
}
/*---------------------------------------------------------------------------------*/
void sciGetJavaSegsBoundingBox(sciPointObj * pSegs, double bounds[6])
{
  getSegsDrawer(pSegs)->getBoundingBox(bounds);
}
/*---------------------------------------------------------------------------------*/
BOOL sciGetJavaAutoResizeMode(sciPointObj * pFigure)
{
 if (getFigureDrawer(pFigure)->getAutoResizeMode())
 {
   return TRUE;
 }
 else
 {
   return FALSE;
 }
}
/*---------------------------------------------------------------------------------*/
void sciGetJavaViewport(sciPointObj * pFigure, int viewport[4])
{
  getFigureDrawer(pFigure)->getViewport(viewport);
}
/*---------------------------------------------------------------------------------*/
