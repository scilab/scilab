/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006-2009 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_demo.c                                                       */
/* desc : function used to launch a demo of a command                     */
/*------------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>

#include "stack-c.h"
#include "sci_demo.h"
#include "sciprint.h"
#include "localization.h"
#include "MALLOC.h"
#include "Scierror.h"

int sci_demo( char * fname, char * code, BOOL flagx )
{
  int mlhs = 0;
	int mrhs = 1;
	int ibegin = 1;
	int stackPointer;
	int n1 = 1;
	static char name[] = "execstr" ;

	/* Include the code inside a function */
	/* The code will then be launched inside a scope */
	/* which won't disturb upper scopes. */
	/* The final code is: */
	/* function %launchGraphicDemo(), <<code>> endfunction; %launchGraphicDemo(); clear %launchGraphicDemo;*/

	static char scopeOpening[] = "function %launchGraphicDemo(), ";
	static char scopeClosing[] = " endfunction; %launchGraphicDemo(); clear %launchGraphicDemo;";
	int launchCodeLength = (int) (strlen(scopeOpening) + strlen(code) + strlen(scopeClosing));

	char * finalCode = MALLOC((launchCodeLength + 1) * sizeof(char));
	if (finalCode == NULL)
	{
		Scierror(999, _("%s: No more memory.\n"),fname);
		LhsVar(1) = 0; 
		return -1;
	}

	/* Add scope to the code and put it inside finalCode */
	sprintf(finalCode, "%s%s%s", scopeOpening, code, scopeClosing);

	Nbvars = 0 ;

	/* Put the code on the stack */
	CreateVar(1,STRING_DATATYPE, &launchCodeLength, &n1, &stackPointer);
	strcpy(cstk(stackPointer), finalCode);

	FREE(finalCode);
	
	/* back conversion to Scilab coding */
	Convert2Sci(1);
	
	/* execute the Scilab execstr function */
	if (flagx){
		sciprint("\n");
		sciprint(_("Demo of %s().\n"), fname);
		sciprint("========================================");
		sciprint("\n");
		sciprint("%s\n", code);
		sciprint("\n");
	}
	
	/* execute it */
	C2F(recu).krec=-1; /* added to avoid recursion errors */
	SciString(&ibegin,name,&mlhs,&mrhs);
	
	/* check if an error has occured while running a_function */
	LhsVar(1) = 0; 
	return 0;
}
