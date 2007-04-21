/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h> /* FILE */
#include "mputstr.h"
#include "filesmanagement.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
void C2F(mputstr) (int *fd, char *str, double *res, int *ierr)
{   
	FILE *fa=NULL;
	fa = GetFileOpenedInScilab(*fd);
	if (!fa) 
	{
		sciprint("No input file \r\n");
		*ierr=1;
	}
	else 
	{
		fprintf(fa,"%s",str);
		(*res) = (*ierr) =ferror(fa);
	}
}
/*-----------------------------------------------------------------------------------*/
