/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

#include "machine.h"
#include "stack-c.h"
#include "sci_demo.h"
#include "sciprint.h"
#include "localization.h"

int sci_demo( char * fname, char * code, int * flagx )
{
  int mlhs=0,mrhs=1,ibegin=1, l1, m1= (int)strlen(code), n1=1;
	static char name[] = "execstr" ;
	Nbvars = 0 ;
	
	CreateVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	strcpy(cstk(l1),code);
	
	/* back conversion to Scilab coding */
	Convert2Sci(1);
	
	/* execute the Scilab execstr function */
	if ( *flagx == 1){
		sciprint("\n");
		sciprint(_("Demo of %s().\n"),fname);
		sciprint("========================================");
		sciprint("\n");
		sciprint("%s\n",code);
		sciprint("\n");
	}
	
	C2F(recu).krec=-1; /* added to avoid recursion errors */
	SciString(&ibegin,name,&mlhs,&mrhs);
	
	/* check if an error has occured while running a_function */
	LhsVar(1) = 0; 
	return 0;
}
