/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/

#include "gw_polynomials.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static int sci_notused(char *fname,unsigned long fname_len)
{
	/* not used */
	return 0;
}
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
{sci_poly_primitive,"poly"},
{sci_roots,"roots"},
{sci_degree,"degree"},
{sci_coeff,"coeff"},
{sci_notused,""}, /* not used but required primitives order (short cut fin,fun) */
{sci_pppdiv,"pppdiv"},
{sci_simp,"simp"},
{sci_psum,"sum"},
{sci_pprod,"prod"},
{sci_pdiag,"diag"},
{sci_ptriu,"triu"},
{sci_ptril,"tril"},
{sci_bezout,"bezout"},
{sci_sfact,"sfact"},
{sci_simp_mode,"simp_mode"},
{sci_varn,"varn"},
{sci_cleanp,"cleanp"}
};
/*--------------------------------------------------------------------------*/
int gw_polynomials(void)
{
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
