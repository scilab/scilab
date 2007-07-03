/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_cacsd2.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_xxxx)_PARAMS((char *fname,unsigned long fname_len))
{
	/* not used */
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_xxxx),"xxxx"}, /* not used */
	{C2F(sci_ppol),"ppol"},
	{C2F(sci_tzer),"tzer"},
	{C2F(sci_freq),"freq"},
	{C2F(sci_ltitr),"ltitr"},
	{C2F(sci_rtitr),"rtitr"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd2)(void)
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
