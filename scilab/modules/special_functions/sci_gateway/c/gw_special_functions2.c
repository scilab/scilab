/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h> /*pour strcmp */
#include <math.h>
/*-----------------------------------------------------------------------------------*/
#include "gw_special_functions2.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
#include "../../../elementaries_functions/includes/xerhlt.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]={ 
  {sci_legendre, "legendre"},
  {sci_beta, "beta"},
  {sci_besseli,"besseli"},
  {sci_besselj,"besselj"},
  {sci_besselk,"besselk"},
  {sci_bessely,"bessely"},
  {sci_besselh,"besselh"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_special_functions2)(void)
{
	Rhs = Max(0, Rhs);
	if (setjmp_slatec_jmp_env()) 
	{ 
		Scierror(999,"%s: arguments must be positive \n", Tab[Fin-1].name);
		return 0;
	}
	callFunctionFromGateway(Tab);

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
