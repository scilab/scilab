/*-----------------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include "delfile.h"
#include "filesmanagement.h"
/*-----------------------------------------------------------------------------------*/
void C2F(delfile)(integer *fd)
{
	if (*fd>=0 && *fd<GetMaximumFileOpenedInScilab() ) 
	{
		SetFileOpenedInScilab(*fd,(FILE*) NULL);
		SetSwapStatus(*fd,0);
		SetFileTypeOpenedInScilab(*fd,0);
		SetFileModeOpenedInScilab(*fd,0);
		FreeFileNameOpenedInScilab(*fd);
	}
}
/*-----------------------------------------------------------------------------------*/
