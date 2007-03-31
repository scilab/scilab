
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_polynomials.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Polynomials_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Polynomials_Interf f;    /** function **/
	char *name;      /** its name **/
} PolynomialsTable;
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
#ifdef _MSC_VER
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
