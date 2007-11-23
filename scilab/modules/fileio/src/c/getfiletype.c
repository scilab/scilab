/*--------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include "getfiletype.h"
#include "filesmanagement.h"
/*--------------------------------------------------------------------------*/
void C2F(getfiletype)(integer *fd, integer *type, integer *ierr)
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
	*type = GetFileTypeOpenedInScilab(*fd);
	*ierr=0;
}
/*--------------------------------------------------------------------------*/
