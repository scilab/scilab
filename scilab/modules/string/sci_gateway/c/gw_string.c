/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET , Cong WU
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "gw_string.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
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
	{C2F(sci_stripblanks),"stripblank"},
	{C2F(sci_strcmp),"strcmp"},
	{C2F(sci_isletter),"isletter"},
	{C2F(sci_isdigit),"isdigit"},
	{C2F(sci_isalphanum),"isalphanum"},
	{C2F(sci_isascii),"isascii"},
	{C2F(sci_strcspn),"strcspn"},
	{C2F(sci_strncpy),"strncpy"},
	{C2F(sci_strrchr),"strrchr"},
	{C2F(sci_strchr),"strchr"},
	{C2F(sci_strstr),"strstr"},
	{C2F(sci_strrev),"strrev"},
	{C2F(sci_strtok),"strtok"},
	{C2F(sci_strspn),"strspn"},
	{C2F(sci_strtod),"strtod"},
	{C2F(sci_regexp),"regexp"}

};
/*--------------------------------------------------------------------------*/
int gw_string(void)
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/

