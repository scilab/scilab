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

void DrawAxis(double *xbox, double *ybox, integer *Indices, integer style) ;

int triangleSort(integer *polyxin, integer *polyyin, integer *fillin, integer *polyx, integer *polyy, integer *fill);

int scilab_shade(integer *polyx, integer *polyy, integer *fill, integer polysize, integer flag) ;

void BBoxToval(double *x, double *y, double *z, integer ind, double bbox[6] ) ;

#endif /* _PLO3D_H_ */
