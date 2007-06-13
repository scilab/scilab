/*------------------------------------------------------------------------*/
/* file: FigureScilabJniCall.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Call from Java to figure drawing code using JNI                 */
/*------------------------------------------------------------------------*/

#include "FigureScilabCall.h"
#include "DrawableFigure.h"
#include "../DrawingBridge.h"
#include "../getHandleDrawer.h"

extern "C"
{
#include "GetProperty.h"
}

/*-----------------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_org_scilab_modules_graphics_figureDrawing_FigureScilabCall_displayFigure( JNIEnv * env, jobject obj, jint figureId )
{
  int figIndex = (int) figureId ;
  sciPointObj * curFig = sciIsExistingFigure(&figIndex) ;
  if ( curFig == NULL ) { return ;}
  ((sciGraphics::DrawableFigure*)sciGraphics::getHandleDrawer(curFig))->drawInContext() ;
}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_org_scilab_modules_graphics_figureDrawing_FigureScilabCall_redrawFigure( JNIEnv * env, jobject obj, jint figureId )
{
  int figIndex = (int) figureId ;
  sciPointObj * curFig = sciIsExistingFigure(&figIndex) ;
  if ( curFig == NULL ) { return ;}
  redrawHierarchy(curFig) ;
}
/*-----------------------------------------------------------------------------------*/

