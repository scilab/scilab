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

extern "C"
{
#include "GetProperty.h"
}

/*-----------------------------------------------------------------------------------*/
void displayFigure(int figureId)
{
  startGraphicDataDisplaying();

  sciPointObj * curFig = sciIsExistingFigure(&figureId) ;
  if ( curFig == NULL )
  {
    endGraphicDataDisplaying();
    return ;
  }
  
  if ( sciGetIsReadyForRendering(curFig) )
  {
    (sciGraphics::getFigureDrawer(curFig))->drawInContext() ;
  }

  endGraphicDataDisplaying();
}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
void redrawFigure(int figureId)
{
  startGraphicDataDisplaying();

  sciPointObj * curFig = sciIsExistingFigure(&figureId) ;
  if ( curFig == NULL )
  {
    endGraphicDataDisplaying();
    return ;
  }

  if ( sciGetIsReadyForRendering(curFig) )
  {
    redrawHierarchy(curFig) ;
  }

  endGraphicDataDisplaying();
}
/*-----------------------------------------------------------------------------------*/

