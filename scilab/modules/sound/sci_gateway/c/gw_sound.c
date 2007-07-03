/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_sound.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]={
	{ sci_savewave, "savewave"},
	{ sci_loadwave, "loadwave"},
	{ sci_Playsound,"PlaySound"},
	{ sci_Beep,"beep"}
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_sound)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
