/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_signal.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_ffir),"ffir"},
	{C2F(sci_fft),"fft"},
	{C2F(sci_fiir),"fiir"},
	{C2F(sci_corr),"corr"},
	{C2F(sci_rpem),"rpem"},
	{C2F(sci_amell),"amell"},
	{C2F(sci_delip),"delip"},
	{C2F(sci_remez),"remez"},
	{C2F(sci_syredi),"syredi"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_signal)()
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
