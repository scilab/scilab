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

/*------------------------------------------------------------------------
*    Graphic library
--------------------------------------------------------------------------*/

#ifndef _CHAMP_H_
#define _CHAMP_H_
#include "dynlib_graphics.h"
#include "machine.h" /* C2F */

GRAPHICS_IMPEXP int C2F(champ) (double *x, double *y, double *fx, double *fy, int *n1, int *n2, char *strflag, double *brect, double *arfact, int lstr) ;
GRAPHICS_IMPEXP int C2F(champ1) (double *x, double *y, double *fx, double *fy, int *n1, int *n2, char *strflag, double *brect, double *arfact, int lstr) ;

GRAPHICS_IMPEXP void champg(char *name, int colored, double *x, double *y, double *fx, double *fy, int *n1,
                            int *n2, char *strflag, double *brect, double *arfact, int lstr);


#endif
