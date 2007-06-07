/*------------------------------------------------------------------------*/
/* file: GetJavaProperty.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface functions between between GetProperty functions and   */
/*        the C++/Java part of module                                     */
/*------------------------------------------------------------------------*/

#ifndef _GET_JAVA_PROPERTY_H_
#define _GET_JAVA_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ObjectStructure.h"

/**
 * Get the colormap of a figure. The colormap is stored in java
 * so we need to access it.
 * @param pFigure figure from which we want the colormap
 * @param rgbMat matrix of size nbColor x 3 conating the 3 channels
 *               RGB of each color.
 */
void sciGetJavaColormap( sciPointObj * pFigure, double rgbMat[] ) ;

#ifdef __cplusplus
}
#endif

#endif /* _GET_JAVA_PROPERTY_H_ */