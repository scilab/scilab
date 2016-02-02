/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
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
/* file: GrayPlot.h                                                       */
/* desc : Functions to compute and draw a grayplot                        */
/*------------------------------------------------------------------------*/

#ifndef _PLOT2D_H_
#define _PLOT2D_H_

#include "dynlib_graphics.h"

GRAPHICS_IMPEXP int re_index_brect(double* brect, double* drect);

GRAPHICS_IMPEXP BOOL strflag2axes_properties(int iSubwinUID, char * strflag);

GRAPHICS_IMPEXP int plot2dn(int ptype, char *logflags, double *x, double *y, int *n1, int *n2, int *style, char *strflag, char *legend, double *brect, int *aaint, BOOL flagNax, int lstr1, int lstr2);

#endif /* _PLOT2D_H_ */
