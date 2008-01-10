/*--------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include <stdio.h> /* FILE */
#include "mputstr.h"
#include "filesmanagement.h"
#include "sciprint.h"
#include "machine.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void C2F(mputstr) (int *fd, char *str, double *res, int *ierr)
{   
	FILE *fa=NULL;
	fa = GetFileOpenedInScilab(*fd);
	if (!fa) 
	{
		sciprint(_("%s: Error while opening, reading or writing '%s'.\n"),"mputstr",GetFileNameOpenedInScilab(*fd));
		*ierr=1;
	}
	else 
	{
		fprintf(fa,"%s",str);
		(*res) = (*ierr) =ferror(fa);
	}
}
/*--------------------------------------------------------------------------*/
