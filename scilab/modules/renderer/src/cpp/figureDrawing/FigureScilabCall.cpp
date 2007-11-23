/*------------------------------------------------------------------------*/
/* file: FigureScilabJniCall.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Call from Java to figure drawing code using JNI                 */
/*------------------------------------------------------------------------*/

#include "FigureScilabCall.h"
#include "DrawableFigure.h"
#include "DrawingBridge.h"
#include "getHandleDrawer.h"
#include "GraphicSynchronizerInterface.h"

#include <iostream>

extern "C"
{
#include "WindowList.h"
}

/*--------------------------------------------------------------------------*/
void displayFigure(int figureId)
{
  startGraphicDataDisplaying();

  sciPointObj * curFig = getFigureFromIndex(figureId) ;
  if ( curFig == NULL )
  {
    endGraphicDataDisplaying();
    return ;
  }
 
 (sciGraphics::getFigureDrawer(curFig))->drawInContext() ;
 
  endGraphicDataDisplaying();
}
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
void redrawFigure(int figureId)
{
  startGraphicDataDisplaying();

  sciPointObj * curFig = getFigureFromIndex(figureId) ;
  if ( curFig == NULL )
  {
    endGraphicDataDisplaying();
    return ;
  }

  redrawHierarchy(curFig) ;

  endGraphicDataDisplaying();
}
/*--------------------------------------------------------------------------*/

