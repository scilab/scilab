/*------------------------------------------------------------------------*/
/* file: FigureScilabCall.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Call from Java to figure drawing code using JNI                 */
/*------------------------------------------------------------------------*/

#ifndef _FIGURE_SCILAB_CALL_
#define _FIGURE_SCILAB_CALL_

#include <jni.h>
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_scilab_modules_renderer_figureDrawing_FigureScilabCall_displayFigure( JNIEnv *env , jobject obj, jint figureId);

JNIEXPORT void JNICALL Java_org_scilab_modules_renderer_figureDrawing_FigureScilabCall_redrawFigure( JNIEnv * env, jobject obj, jint figureId ) ;
#ifdef __cplusplus
}
#endif
#endif /* _FIGURE_SCILAB_CALL_ */
