/*------------------------------------------------------------------------*/
/* file: ColorMapManagement.h                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : Several algorithm to deal with colormaps                        */
/*------------------------------------------------------------------------*/

#ifndef _COLOR_MAP_MANAGEMENT_H_
#define _COLOR_MAP_MANAGEMENT_H_

#include "ObjectStructure.h"

int LinearScaling2Colormap( sciPointObj * pobj ) ;

/**
* Convert a colormap to a black and white one of the same size.
* Each color is replaced by its grey scale.
* of the source colormap.
* @param bwColorMap destination of the function
* @param colorMap   source colormap. The two colormaps might be actually the same
*                   if a copy is not needed.
*/
void convertColorMap2BW( double * bwColorMap, double * colorMap, int colorMapSize ) ;


#endif /* _COLOR_MAP_MANAGEMENT_H_ */
