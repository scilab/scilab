/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __SOMEINTERP_H__
#define __SOMEINTERP_H__

/**
 * TODO : comment 
 * @param x
 * @param val
 * @param dim
 * @param n
 * @param xp
 * @param yp
 * @param np
 * @param outmode
 */
void nlinear_interp(double **x , double val[], int dim[], int n,
		    double **xp, double yp[], int np, int outmode, 
					double u[], double v[], int ad[], int k[]);

#endif /* __SOMEINTERP_H__ */
