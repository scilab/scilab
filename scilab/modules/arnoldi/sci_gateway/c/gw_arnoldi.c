/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
/*-----------------------------------------------------------------------------------*/
#include "gw_arnoldi.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]={
  {C2F(intdsaupd),"dsaupd"},
  {C2F(intdnaupd),"dnaupd"},
  {C2F(intznaupd),"znaupd"},
  {C2F(intdseupd),"dseupd"},
  {C2F(intdneupd),"dneupd"},
  {C2F(intzneupd),"zneupd"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_arnoldi)(void)
{
	Rhs = Max(0, Rhs);
	
	callFunctionFromGateway(Tab);

	return 0;
}
/*-----------------------------------------------------------------------------------*/


