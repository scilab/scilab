/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <string.h> /* strlen */
#include "getfileinfo.h"
#include "filesmanagement.h"
/*-----------------------------------------------------------------------------------*/
void C2F(getfileinfo)(integer *fd, FILE *fa, integer *swap2, integer *type, integer *mode, char *filename, integer *lf, integer *ierr)
{
	if (*fd<0 || *fd>=GetMaximumFileOpenedInScilab() ) 
	{
		*ierr=1;
		return;
	}
	if ( GetFileTypeOpenedInScilab(*fd) == 0 ) 
	{
		*ierr=2;
		return;
	}
	fa = GetFileOpenedInScilab(*fd);

	*swap2 = GetSwapStatus(*fd);
	*type = GetFileTypeOpenedInScilab(*fd);
	*mode = GetFileModeOpenedInScilab(*fd);
	strcpy(filename,GetFileNameOpenedInScilab(*fd));
	*lf=(int)strlen(filename);
	*ierr=0;
}
/*-----------------------------------------------------------------------------------*/
