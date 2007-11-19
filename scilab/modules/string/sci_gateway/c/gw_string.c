/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_string.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_length),"length"},
	{C2F(sci_part),"part"},
	{C2F(sci_string),"string"},
	{C2F(sci_convstr),"convstr"},
	{C2F(sci_emptystr),"emptystr"},
	{C2F(sci_str2code),"str2code"},
	{C2F(sci_code2str),"code2str"},
	{C2F(sci_strcat),"strcat"},
	{C2F(sci_strindex),"strindex"},
	{C2F(sci_strsubst),"strsubst"},
	{C2F(sci_ascii),"ascii"},
	{C2F(sci_grep),"grep"},
	{C2F(sci_tokens),"tokens"},
	{C2F(sci_strsplit),"strsplit"},
	{C2F(sci_stripblanks),"stripblank"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_string)(void)
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
