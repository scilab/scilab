/*------------------------------------------------------------------------*/
/* file: drawPolylineEntity.h                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a Polyline handle             */
/*------------------------------------------------------------------------*/

#ifndef _DRAW_POLYLINE_ENTITY_H_
#define _DRAW_POLYLINE_ENTITY_H_

#include "ObjectStructure.h"

/**
* Routine which draw a polyline object
* @param pObj the pointer on the polyline object
*/
int drawPolylineEntity( sciPointObj * pObj ) ;

/**
 * 
 * BuildXYZvectForClipping_IfNanOrLogON : this function is used for polylines to determine if we have to cut the polyline data for 2 reasons:
 * - clipping is forced because we have a Nan inside the data columns
 * - clipping is forced because we have a log scale on an axis and polyline data < 0 on this axis (clipping on X11 or Win is based on pixel we compute so if
 * the algo tried to evaluate log(-5.6) (for example) and then we tried to make a Xscale (or logXscale) on it, it doesn't work at all!!
 * What we do is: we cut the polyline into several polylines and reject Nan values and data<0 if axis is in log scale
 * @author F.Leray 02.11.04
 */
int BuildXYZvectForClipping_IfNanOrLogON( sciPointObj *   ppolyline,
                                          sciPointObj *   psubwin  ,
                                          int         *   nb_curves,
                                          double      *** xvect    ,
                                          double      *** yvect    ,
                                          double      *** zvect    ,
                                          int         **  curves_size ) ;

void Plo2dTo3d(integer type, integer *n1, integer *n2, double x[], double y[], double z[], double xOut[], double yOut[], double zOut[]) ;

#endif /* _DRAW_POLYLINE_ENTITY_H_ */
