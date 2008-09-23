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
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "mseek.h"
#include "filesmanagement.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
#if (defined(sun) && !defined(SYSV)) 
char *strerror (int errcode);
#endif
/*--------------------------------------------------------------------------*/
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif 
/*--------------------------------------------------------------------------*/
void C2F(mseek) (int *fd, int *offset, char *flag, int *err)
{     
	int iflag;
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
	int irep;
#endif
	FILE *fa= GetFileOpenedInScilab(*fd);
	*err=0;
	if ( fa == (FILE *) 0 ) 
	{
		char *filename = GetFileNameOpenedInScilab(*fd);
		if (filename)
		{
			sciprint(_("%s: Error while opening, reading or writing '%s'.\n"),"mseek",filename);
		}
		else
		{
			sciprint(_("%s: Error while opening, reading or writing.\n"),"mseek");
		}
		
		*err=1;
		return;
	}
	if ( strncmp(flag,"set",3)==0 ) 
		iflag = SEEK_SET;
	else if ( strncmp(flag,"cur",3)==0 ) 
		iflag = SEEK_CUR;
	else if ( strncmp(flag,"end",3)==0 ) 
		iflag = SEEK_END;
	else 
	{
		sciprint(_("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"),"mseek",3,"set","cur","end");
		*err=1;
		return;
	}
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
	irep = fseek(fa,(long) *offset,iflag) ;
	if ( irep != 0 ) 
	{
		sciprint(strerror(irep));
		*err=1;
	}
	else
		*err=0;
#else
	if (fseek(fa,(long) *offset,iflag) == -1 ) 
	{
		int errnum=errno; /* global variable produced by fseek */
		sciprint(_("%s: An error occurred in %s: errno=%s\n"),"mseek","fseek",strerror(errnum));
		*err=1;
	}
	else 
		*err=0;
#endif
}
/*--------------------------------------------------------------------------*/
