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
  startGraphicDataReading();
  sciPointObj * curFig = getFigureFromIndex(figureId) ;
  endGraphicDataReading();

  if ( curFig == NULL )
  {
    return ;
  }

  startFigureDataDisplaying(curFig);
 (sciGraphics::getFigureDrawer(curFig))->drawInContext() ;
  endFigureDataDisplaying(curFig);
}
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
void redrawFigure(int figureId)
{
  startGraphicDataReading();
  sciPointObj * curFig = getFigureFromIndex(figureId) ;
  endGraphicDataReading();

  if ( curFig == NULL )
  {
    return ;
  }

  startFigureDataDisplaying(curFig);
  redrawHierarchy(curFig);
  endFigureDataDisplaying(curFig);
}
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
void exportFigure(int figureId, char * fileName, int fileType)
{

}
/*--------------------------------------------------------------------------*/
