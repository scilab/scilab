/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include "mgetstr.h"
#include "filesmanagement.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
void C2F(mgetstr) (integer *fd, char **start, integer *n, integer *ierr)
{ 
	int count=0;
	FILE *fa=NULL;
	fa = GetFileOpenedInScilab(*fd);
	*ierr=0;
	if (fa)
	{ 
		*start= (char *) MALLOC((*n+1)*sizeof(char));
		if ( *start == (char *) 0)
		{       
			sciprint(_("No more memory \n"));
			*ierr=1;
			return;
		}
		count=(int)fread(*start,sizeof(char),*n,fa);
		(*start)[*n]='\0';
		if ( count != *n ) 
		{
			*ierr = - count -1;
		}
		return;
	}
	sciprint(_("No input file \n"));
	*ierr=1;
}
/*-----------------------------------------------------------------------------------*/
void C2F(mgetstr1) (integer *fd, char *start, integer *n, integer *ierr)
{ 
	int count=0;
	FILE *fa=NULL;
	fa = GetFileOpenedInScilab(*fd);
	*ierr=0;
	if (fa)
	{ 
		count=(int)fread(start,sizeof(char),*n,fa);
		start[*n]='\0';
		if ( count != *n ) 
		{
			*ierr = - count -1;
		}
		return;
	}
	sciprint(_("No input file \n"));
	*ierr=1;
}
/*-----------------------------------------------------------------------------------*/
