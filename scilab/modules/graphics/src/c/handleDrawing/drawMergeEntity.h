/*------------------------------------------------------------------------*/
/* file: drawMergeEntity.h                                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a Merge handle                */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_MERGE_ENTITY_H_
#define _DRAW_MERGE_ENTITY_H_

#include "ObjectStructure.h"

/**
 * Routine which draw a merge object.
 * @param pObj the pointer on the merge object
 */
int drawMergeEntity( sciPointObj * pObj ) ;

void DrawMerge3d( sciPointObj * psubwin, sciPointObj * pmerge, int * DPI ) ;  /* DRAWINGS */

/**
 * get the coordinates of the vertices of a facet in a FAC3D object
 * @param[in/out] pobj       surface object of type Fac3d
 * @param         facetIndex index of the facet in the object
 * @param[out]    verticesX  list of X coordinates of the vertices
 * @param[out]    verticesY  list of Y coordinates of the vertices
 * @param[out]    verticesZ  list of Z coordinates of the vertices
 * @param[out]    zOriginal  pointer on the Z coordinate of the first vertex in the fac3d object
 */
void retrieveFacetVertices( sciPointObj *  pobj       ,
                            int            facetIndex ,
                            double         verticesX[],
                            double         verticesY[],
                            double         verticesZ[],
                            double      ** zOriginal   ) ;

#endif /* _DRAW_MERGE_ENTITY_H_ */
