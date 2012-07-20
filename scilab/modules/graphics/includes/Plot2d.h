/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
/* file: GrayPlot.h                                                       */
/* desc : Functions to compute and draw a grayplot                        */
/*------------------------------------------------------------------------*/

#ifndef _PLOT2D_H_
#define _PLOT2D_H_

#include "dynlib_graphics.h"

GRAPHICS_IMPEXP int re_index_brect(double * brect, double * drect);

GRAPHICS_IMPEXP BOOL strflag2axes_properties(char * psubwinUID, char * strflag);

GRAPHICS_IMPEXP int plot2dn(int ptype,char *logflags,double *x,double *y,int *n1,int *n2,int *style,char *strflag,char *legend,double *brect,int *aaint,BOOL flagNax, int lstr1,int lstr2);

#endif /* _PLOT2D_H_ */
