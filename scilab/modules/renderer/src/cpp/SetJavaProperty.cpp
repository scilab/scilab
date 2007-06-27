/*------------------------------------------------------------------------*/
/* file: SetJavaProperty.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface functions between between SetProperty functions and   */
/*        the C++/Java part of module                                     */
/*------------------------------------------------------------------------*/

#include "SetJavaProperty.h"
#include "getHandleDrawer.h"
#include "figureDrawing/DrawableFigure.h"

using namespace sciGraphics ;

/*------------------------------------------------------------------------------------------*/
void sciSetJavaColormap( sciPointObj * pFigure, const double rgbMat[], int nbColor )
{
  ((DrawableFigure *)getHandleDrawer(pFigure))->setColorMap( rgbMat, nbColor ) ;
}
/*------------------------------------------------------------------------------------------*/
void sciSetJavaFigureSize( sciPointObj * pFigure, const int size[2] )
{
  getFigureDrawer(pFigure)->setSize(size) ;
}
/*------------------------------------------------------------------------------------------*/
void sciSetJavaWindowSize( sciPointObj * pFigure, const int size[2] )
{
  getFigureDrawer(pFigure)->setWindowSize(size) ;
}
/*------------------------------------------------------------------------------------------*/
void sciSetJavaWindowPosition( sciPointObj * pFigure, const int pos[2] )
{
  getFigureDrawer(pFigure)->setWindowPosition(pos) ;
}
/*------------------------------------------------------------------------------------------*/
void sciSetJavaInfoMessage( sciPointObj * pFigure, const char * infoMessage )
{
  getFigureDrawer(pFigure)->setInfoMessage(infoMessage);
}
/*------------------------------------------------------------------------------------------*/