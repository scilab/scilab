/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
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
				sciprint(_("%s: Cannot close file whose descriptor is %d: File is not active.\n"),"mclose",fd1);
			}
		}
		else 
		{
			*res = -1.0;
			sciprint(_("%s: Cannot close file whose descriptor is %d: No file to close.\n"),"mclose",fd1);
		}
	}
}
/*--------------------------------------------------------------------------*/
