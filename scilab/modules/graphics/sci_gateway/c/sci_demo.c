/*------------------------------------------------------------------------*/
/* file: sci_demo.c                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : function used to laubch a demo of a command                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sci_demo.h"
#include "sciprint.h"

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
		sciprint("\r\n");
		sciprint("Demo of %s()\r\n",fname);
		sciprint("========================================");
		sciprint("\r\n");
		sciprint("%s\r\n",code);
		sciprint("\r\n");
	}
	
	C2F(recu).krec=-1; /* added to avoid recursion errors */
	SciString(&ibegin,name,&mlhs,&mrhs);
	
	/* check if an error has occured while running a_function */
	LhsVar(1) = 0; 
	return 0;
}
