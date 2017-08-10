/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */


/*------------------------------------------------------------------------*/
/* file: Fec.h                                                            */
/* desc : Functions used to draw fec objects                              */
/*------------------------------------------------------------------------*/

#ifndef _FEC_H_
#define _FEC_H_
#include "BOOL.h"
#include "machine.h"
/**
 *  Iso contour with grey level or colors
 *  for a function defined by finite elements
 *  (f is linear on triangles)
 *  we give two versions of the function :
 *     - a quick version wich only fill triangles according to the average
 *     value of f on a triangle (no more such version now ?)
 *     - and a slow version but more sexy which use the fact that f is linear
 *     on each triangle.
 *  Nodes (x[no],y[no])
 *  Triangles (Matrix: [ numero, no1,no2,no3,iflag;...]
 *  func[no] : Function value on Nodes.
 *  Nnode : number of nodes
 *  Ntr   : number of triangles
 *  strflag,legend,brect,aint : see plot2d
 *  zminmax   : to set (optionaly) the min and max level
 *  colminmax : to set (optionaly) the first and last color to use
 *
 *  modified by Bruno Pincon 01/02/2001 for gain in speed and added
 *  possibilities to set zmin, zmax by the user and also to set the
 *  first and last color of the colormap (Bruno.Pincon@iecn.u-nancy.fr)
 */
int C2F(fec)(double *x, double *y, double *triangles, double *func, int *Nnode, int *Ntr, int *Nvertex,
             char *strflag, char *legend, double *brect, int *aaint, double *zminmax,
             int *colminmax, int *colout, BOOL with_mesh, BOOL flagNax, int lstr1, int lstr2);

#endif /* _FEC_H_ */
