/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2000 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
*    Graphic library
*--------------------------------------------------------------------------*/

#ifndef _PLO3D_H_
#define _PLO3D_H_

void DrawAxis(double *xbox, double *ybox, int *Indices, int style) ;

int triangleSort(int *polyxin, int *polyyin, int *fillin, int *polyx, int *polyy, int *fill);

int scilab_shade(int *polyx, int *polyy, int *fill, int polysize, int flag) ;

void BBoxToval(double *x, double *y, double *z, int ind, double bbox[6] ) ;

#endif /* _PLO3D_H_ */
