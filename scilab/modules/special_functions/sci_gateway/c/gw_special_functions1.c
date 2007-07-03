/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
/*-----------------------------------------------------------------------------------*/
#include "gw_special_functions1.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]={ 
  {C2F(sci_oldbesseli),"oldbesseli"},
  {C2F(sci_oldbesselj), "oldbesselj"},
  {C2F(sci_oldbesselk),"oldbesselk"},
  {C2F(sci_oldbessely),"oldbessely"},
  {C2F(sci_gamma),"gamma"},
  {C2F(sci_lgamma),"gammaln"},
  {C2F(sci_dlgamma),"dlgamma"},
  {C2F(sci_calerf),"calerf"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_special_functions1)(void)
{
	Rhs = Max(0, Rhs);

	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
