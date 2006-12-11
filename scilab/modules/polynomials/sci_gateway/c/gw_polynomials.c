#include "gw_polynomials.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_poly) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_roots) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_degree) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_coeff) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_eval) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_pppdiv) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_simp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_psum) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_pprod) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_pdiag) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ptriu) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ptril) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_bezout) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sfact) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_simp_mode) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_varn) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_cleanp) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_polynomials)(void);
/*-----------------------------------------------------------------------------------*/
static PolynomialsTable Tab[]=
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
/*-----------------------------------------------------------------------------------*/
int C2F(gw_polynomials)(void)
{  
#if _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
