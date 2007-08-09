/*------------------------------------------------------------------------*/
/* file: FigureScilabCall.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Call from Java to figure drawing code using JNI                 */
/*------------------------------------------------------------------------*/

#ifndef _FIGURE_SCILAB_CALL_
#define _FIGURE_SCILAB_CALL_

#ifdef __cplusplus
extern "C" {
#endif

void displayFigure(int figureId);

void redrawFigure(int figureId) ;
#ifdef __cplusplus
}
#endif
#endif /* _FIGURE_SCILAB_CALL_ */
