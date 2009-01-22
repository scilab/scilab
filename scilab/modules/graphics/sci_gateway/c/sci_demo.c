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
#include "dynamic_menus.h"

int sci_demo( char * fname, char * code, BOOL flagx )
{
	char name[] = "execstr" ;

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
	
	/* display demo code if needed */
	if (flagx){
		sciprint("\n");
		sciprint(_("Demo of %s().\n"), fname);
		sciprint("========================================");
		sciprint("\n");
		sciprint("%s\n", code);
		sciprint("\n");
	}

	/* the demo will be launched just after the function returns */
	StoreCommand(finalCode);

	FREE(finalCode);
	
	
	/* check if an error has occured while running a_function */
	LhsVar(1) = 0; 
	return 0;
}
