/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include "mtell.h"
#include "filesmanagement.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
void C2F(mtell) (integer *fd, double *offset, integer *err)
{     
	FILE *fa= GetFileOpenedInScilab(*fd);
	if ( fa == (FILE *) 0 ) 
	{
		sciprint("mtell: wrong file logical unit \n");
		*err=1;
		return;
	}
	*err=0;
	*offset = (double) ftell(fa) ;
}
/*-----------------------------------------------------------------------------------*/
