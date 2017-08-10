/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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
