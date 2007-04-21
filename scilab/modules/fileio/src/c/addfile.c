/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include "addfile.h"
#include "filesmanagement.h"
/*-----------------------------------------------------------------------------------*/
void C2F(addfile)(integer *fd, FILE *fa, integer *swap2, integer *type, integer *mode, char *filename, integer *ierr)
{
	if (SetFileNameOpenedInScilab(*fd,filename) == FALSE ) 
	{
		*ierr=1;
		return;
	}
	if (*type==2) SetFileOpenedInScilab(*fd,fa);
	else if (*type==-1) 
		SetFileOpenedInScilab(*fd,stdin);
	else if (*type==-2) 
		SetFileOpenedInScilab(*fd,stdout);
	else if (*type==-3) 
		SetFileOpenedInScilab(*fd,stderr);
	else
		SetFileOpenedInScilab(*fd,(FILE *) 0);

	SetSwapStatus(*fd,*swap2);
	SetFileTypeOpenedInScilab(*fd,*type);
	SetFileModeOpenedInScilab(*fd,*mode);

	*ierr=0;
}
/*-----------------------------------------------------------------------------------*/

