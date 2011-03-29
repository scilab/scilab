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
#include <stdlib.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{NULL, ""}, //length
	{NULL, ""}, //part
	{NULL, ""}, //string
	{NULL, ""}, //convstr
	{NULL, ""}, //emptystr
	{NULL, ""}, //str2code
	{NULL, ""}, //5 code2str
	{NULL, ""}, //strcat
	{NULL, ""}, //strindex
	{NULL, ""}, //strsubst
	{NULL, ""}, //ascii
	{NULL, ""}, //grep
	{NULL, ""}, //2 tokens
	{NULL, ""}, //strsplit
	{NULL, ""}, //stripblank
	{NULL, ""}, //3 strcmp
	{NULL, ""}, //4 isletter
	{NULL, ""}, //isdigit
	{NULL, ""}, //isalphanum
	{NULL, ""}, //isascii
	{NULL, ""}, //4 strcspn
	{NULL, ""}, //strncpy
	{NULL, ""}, //1 strrchr
	{NULL, ""}, //1 strchr
	{NULL, ""}, //1 strstr
	{NULL, ""}, //1 strrev
	{NULL, ""}, //2 strtok
	{NULL, ""}, //4 strspn
	{NULL, ""}, //1 strtod
	{NULL, ""}  //regexp
};
/*--------------------------------------------------------------------------*/
int gw_string(void)
{
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/

