/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
--------------------------------------------------------------------------*/

#ifndef _CHAMP_H_
#define _CHAMP_H_

int C2F(champ) (double *x, double *y, double *fx, double *fy, int *n1, int *n2, char *strflag, double *brect, double *arfact, int lstr) ; 
int C2F(champ1) (double *x, double *y, double *fx, double *fy, int *n1, int *n2, char *strflag, double *brect, double *arfact, int lstr) ; 

void champg(char *name, int colored, double *x, double *y, double *fx, double *fy, int *n1, 
            int *n2, char *strflag, double *brect, double *arfact, int lstr) ;


#endif
