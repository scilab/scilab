/*--------------------------------------------------------------------------*/
/* Scilab */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "mclose.h"
#include "filesmanagement.h"
#include "delfile.h"
#include "core_math.h" /* Min Max */
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void C2F(mclose) (integer *fd, double *res)
{     
	int fd1;
	int res1 = 1;
	*res = 0.0;
	switch ( *fd )
	{
	case -2 :
		/* closing all opened files */
		for ( fd1=0; fd1< GetMaximumFileOpenedInScilab(); fd1++) {
			if ( GetFileOpenedInScilab(fd1) )
			{
				fclose( GetFileOpenedInScilab(fd1) );
				res1 = ferror( GetFileOpenedInScilab(fd1));
				if (res1 != 0) *res =1;
				C2F(delfile)(&fd1);
			}
		}
		break;
	default :
		fd1 = (*fd == -1 ) ? GetCurrentFileId() : Min(Max(*fd,0),GetMaximumFileOpenedInScilab()-1);
		if ( fd1 != -1 ) 
		{
			if ( GetFileOpenedInScilab(fd1) )
			{
				if (fclose(GetFileOpenedInScilab(fd1)))
				{
					*res = (double)ferror(GetFileOpenedInScilab(fd1));
				}
				C2F(delfile)(&fd1);
			}
			else
			{
				*res = 0.0;
				sciprint(_("File %d not active\n"),fd1);
			}
		}
		else 
		{
			*res = -1.0;
			sciprint(_("No file to close\n"),fd1);
		}
	}
}
/*--------------------------------------------------------------------------*/
