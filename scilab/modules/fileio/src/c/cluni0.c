/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) ENPC
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#include "cluni0.h"
#include "expandPathVariable.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
int C2F(cluni0)(char *in_name, char *out_name, int *out_n, long int lin, long int lout)
{
	char *expandedVar = NULL;
	in_name[lin] = 0;

	expandedVar = expandPathVariable(in_name);
	if (expandedVar)
	{
		strcpy(out_name, expandedVar);
		FREE(expandedVar);
		expandedVar = NULL;
		*out_n = (int) strlen(out_name);
	}
	else
	{
		strcpy(out_name, in_name);
		*out_n = (int) strlen(out_name);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/ 
