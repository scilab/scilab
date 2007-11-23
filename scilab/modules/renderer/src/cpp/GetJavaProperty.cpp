/*------------------------------------------------------------------------*/
/* file: GetJavaProperty.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface functions between between GetProperty functions and   */
/*        the C++/Java part of module                                     */
/*------------------------------------------------------------------------*/

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
BOOL sciGetJavaPixmapMode(sciPointObj * pFigure)
{
  if (getFigureDrawer(pFigure)->getPixmapMode())
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
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
