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

/*------------------------------------------------------------------------------------------*/
void sciGetJavaColormap( sciPointObj * pFigure, double rgbMat[] )
{
  ((DrawableFigure *)getHandleDrawer(pFigure))->getColorMap( rgbMat ) ;
}
/*------------------------------------------------------------------------------------------*/