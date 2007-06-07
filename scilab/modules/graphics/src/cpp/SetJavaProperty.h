/*------------------------------------------------------------------------*/
/* file: SetJavaProperty.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface functions between between SetProperty functions and   */
/*        the C++/Java part of module                                     */
/*------------------------------------------------------------------------*/

#ifndef _SET_JAVA_PROPERTY_H_
#define _SET_JAVA_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ObjectStructure.h"

/**
 * Change the colormap of a figure. The colormap is stored in java
 * so we need to access it.
 * @param pFigure the figure we wants to modify
 * @param rgbMat matrix of size nbColor x 3 conating the 3 channels
 *               RGB of each color.
 * @param nbColor number of color in the new colormap
 */
void sciSetJavaColormap( sciPointObj * pFigure, const double rgbMat[], int nbColor ) ;

#ifdef __cplusplus
}
#endif

#endif /* _SET_JAVA_PROPERTY_H_ */