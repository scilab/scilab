/*------------------------------------------------------------------------*/
/* file: drawSegsEntity.h                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a segs handle                 */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_SEGS_ENTITY_H_
#define _DRAW_SEGS_ENTITY_H_

#include "ObjectStructure.h"

/**
 * Routine which draw a segs object
 * @param pObj the pointer on the segs object
 */
int drawSegsEntity( sciPointObj * pObj ) ;

/**
 * Compute the arrow size which must be used in (dr)(xarrows).
 * @param[in] pSegs object of which we are displaying arrows.
 * @param[in] X coordinates in pixels of the segments
 * @param[in] Y coordinates in pixels of the segments
 * @return    Size to use in xarrows.
 */
int computeRealArrowSize( sciPointObj * pSegs, int nbSegs, int xCoord[], int yCoord[] ) ;

#endif /* _DRAW_SEGS_ENTITY_H_ */
