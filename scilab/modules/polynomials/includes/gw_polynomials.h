/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_POLYNOMIALS__
#define __GW_POLYNOMIALS__
/*--------------------------------------------------------------------------*/
#include "dynlib_polynomials.h"
/*--------------------------------------------------------------------------*/
POLYNOMIALS_IMPEXP int gw_polynomials(void);
/*--------------------------------------------------------------------------*/
POLYNOMIALS_IMPEXP int sci_poly_primitive(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_roots(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_degree(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_coeff(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_pppdiv(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_simp(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_psum(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_pprod(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_pdiag(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_ptriu(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_ptril(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_bezout(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_sfact(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_simp_mode(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_varn(char *fname, unsigned long fname_len);
POLYNOMIALS_IMPEXP int sci_cleanp(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_POLYNOMIALS__ */
/*--------------------------------------------------------------------------*/

