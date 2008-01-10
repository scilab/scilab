/*--------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "mtell.h"
#include "filesmanagement.h"
#include "sciprint.h"
#include "machine.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void C2F(mtell) (integer *fd, double *offset, integer *err)
{     
	FILE *fa= GetFileOpenedInScilab(*fd);
	if ( fa == (FILE *) 0 ) 
	{
		sciprint(_("%s: Error while opening, reading or writing '%s'.\n"),"mtell",GetFileNameOpenedInScilab(*fd));
		*err=1;
		return;
	}
	*err=0;
	*offset = (double) ftell(fa) ;
}
/*--------------------------------------------------------------------------*/
