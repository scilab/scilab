/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <Windows.h>
#include "TermPrintf.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
void TermPrintf_Windows(char *buffer)
{
	if (buffer)
	{
		char *OEM_string = (char*)MALLOC(sizeof(char)*(strlen(buffer)+1));

		/* converts standard chars to OEM string */
		/* windows term output */
		fflush(NULL);
		if (OEM_string)
		{
			CharToOem(buffer,OEM_string);
			printf("%s",OEM_string);
		
		}
		else printf("%s",buffer); 
		fflush(NULL);
	}
}
/*--------------------------------------------------------------------------*/
