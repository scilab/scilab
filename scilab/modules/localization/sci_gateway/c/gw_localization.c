/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_localization.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]=
{
 		{C2F(sci_setlanguage),"setlanguage"},
		{C2F(sci_getlanguage),"getlanguage"},
		{C2F(sci_gettext),"gettext"},
		{C2F(sci_gettext),"_"}
};
			/*	{C2F(sci_getlanguagealias),"getlanguagealias"},
	{C2F(sci_getlanguagestable),"getlanguagestable"},

	{C2F(sci_addtext),"addtext"},
	{C2F(sci_removetext),"removetext"}
*/
/*--------------------------------------------------------------------------*/
int C2F(gw_localization)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
