/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
 * Copyright (C) INRIA - 2005 - Allan Cornet
 * 
 * This file is released into the public domain
 *
 */
#include <stdio.h> /* stderr */

#include "stack-c.h" /* Provide functions to access to the memory of Scilab */
#include "CallScilab.h" /* Provide functions to call Scilab engine */


// Filename: simple_call_scilab.c

int main(void)
{
/****** INITIALIZATION **********/
#ifdef _MSC_VER
	if ( StartScilab(NULL,NULL,NULL) == FALSE )
#else
	if ( StartScilab(getenv("SCI"),NULL,NULL) == FALSE )
#endif
		{
			fprintf(stderr,"Error while calling StartScilab\n");
			return -1;
		}

/****** ACTUAL Scilab TASKS *******/

	SendScilabJob("myMatrix=['sample','for the help']");
	SendScilabJob("disp(myMatrix);"); // Will display !sample  for the help  !
	SendScilabJob("disp([2,3]+[-44,39]);"); // Will display   - 42.    42.  

/****** TERMINATION **********/
	if ( TerminateScilab(NULL) == FALSE ) {
		fprintf(stderr,"Error while calling TerminateScilab\n");
		return -2;
	}
	return 0;
}
