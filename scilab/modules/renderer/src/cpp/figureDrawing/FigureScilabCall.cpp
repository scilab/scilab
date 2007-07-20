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
#include "JniUtils.h"
#include "GetProperty.h"
}

/*-----------------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_org_scilab_modules_renderer_figureDrawing_FigureScilabCall_displayFigure( JNIEnv * env, jobject obj, jint figureId )
{
  startGraphicDataDisplaying();
  int figIndex = (int) figureId ;
  sciPointObj * curFig = sciIsExistingFigure(&figIndex) ;
  if ( curFig == NULL ) { return ;}
  jniSetCurrentEnv(env) ; // we are in a new thread
  if ( sciGetIsReadyForRendering(curFig) )
  {
    (sciGraphics::getFigureDrawer(curFig))->drawInContext() ;
  }
  endGraphicDataDisplaying();
}
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_org_scilab_modules_renderer_figureDrawing_FigureScilabCall_redrawFigure( JNIEnv * env, jobject obj, jint figureId )
{
  startGraphicDataDisplaying();
  int figIndex = (int) figureId ;
  sciPointObj * curFig = sciIsExistingFigure(&figIndex) ;
  if ( curFig == NULL ) { return ;}
  jniSetCurrentEnv(env) ; // we are in a new thread
  if ( sciGetIsReadyForRendering(curFig) )
  {
    redrawHierarchy(curFig) ;
  }
  endGraphicDataDisplaying();
}
/*-----------------------------------------------------------------------------------*/

