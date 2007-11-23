/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_polynomials.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_poly),"poly"},
{C2F(sci_roots),"roots"},
{C2F(sci_degree),"degree"},
{C2F(sci_coeff),"coeff"},
{C2F(sci_eval),"eval"},
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
int C2F(gw_polynomials)(void)
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
