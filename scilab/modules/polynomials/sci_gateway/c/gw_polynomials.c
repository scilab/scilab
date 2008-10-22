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
static int C2F(sci_notused)(char *fname,unsigned long fname_len)
{
	/* not used */
	return 0;
}
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_poly),"poly"},
{C2F(sci_roots),"roots"},
{C2F(sci_degree),"degree"},
{C2F(sci_coeff),"coeff"},
{C2F(sci_notused),""}, /* not used but required primitives order (short cut fin,fun) */
{C2F(sci_pppdiv),"pppdiv"},
{C2F(sci_simp),"simp"},
{C2F(sci_psum),"sum"},
{C2F(sci_pprod),"prod"},
{C2F(sci_pdiag),"diag"},
{C2F(sci_ptriu),"triu"},
{C2F(sci_ptril),"tril"},
{C2F(sci_bezout),"bezout"},
{C2F(sci_sfact),"sfact"},
{C2F(sci_simp_mode),"simp_mode"},
{C2F(sci_varn),"varn"},
{C2F(sci_cleanp),"cleanp"}
};
/*--------------------------------------------------------------------------*/
int gw_polynomials(void)
{
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
