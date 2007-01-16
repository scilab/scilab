/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 2003-2004 INRIA/Djalel Abdemouche
*                  2005-xxxx INRIA/Jean-Baptiste Silvy
--------------------------------------------------------------------------*/

#ifndef _PLO3DN_H_
#define _PLO3DN_H_

#include "ObjectStructure.h"

/**
 * plo3dn function  ( 3D Plotting of surfaces given by z=f(x,y)  )
 */
void C2F (plot3dn) (sciPointObj * pobj, double *x, double *y, double *z,
                    integer * p, integer * q, int *DPI) ;
/**
 * display a fac3d object. Apply the painter algorithm to the facets
 * and draw them with the right color (hidden or not).
 * @param pobj the fac3d object
 * @param x the matrice of the x coordiantes of facets
 * @param y the matrice of the y coordiantes of facets
 * @param z the matrice of the z coordiantes of facets
 * @param cvect the color vector
 * @param p the number of vertice the facests contains
 * @param q the number of facets
 * @param DPI not yet identified (deals with display probably)
 */
void C2F (fac3dn) (sciPointObj * pobj, double *x, double *y, double *z,
                   double * cvect, integer * p, integer * q, int *DPI) ;

#endif /* _PLO3DN_H_ */
