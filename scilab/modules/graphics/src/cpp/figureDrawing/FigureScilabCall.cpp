/*------------------------------------------------------------------------*/
/* file: FigureScilabJniCall.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Call from Java to figure drawing code using JNI                 */
/*------------------------------------------------------------------------*/

#include "FigureScilabCall.h"
#include "DrawableFigure.h"
#include "../getHandleDrawer.h"
#include <stdio.h>
extern "C"
{
#include "CurrentObjectsManagement.h"
#include "sciprint.h"
}

/*-----------------------------------------------------------------------------------*/
JNIEXPORT void JNICALL Java_org_scilab_modules_graphics_figureDrawing_FigureScilabCall_redrawFigure( JNIEnv * env, jobject obj, jint figureId )
{
  sciprint("Redraw the object.\n") ;
  ((sciGraphics::DrawableFigure*)sciGraphics::getHandleDrawer(sciGetCurrentFigure()))->drawInContext() ;
}
/*-----------------------------------------------------------------------------------*/
