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
#include "diary.h"
#include "stack-c.h"
#include "filesmanagement.h"
#include "mclose.h"
/*--------------------------------------------------------------------------*/
int getdiary(void)
{
	return C2F(iop).wio;
}
/*--------------------------------------------------------------------------*/
void setDiaryId(int fd)
{
	C2F(iop).wio = fd;
}
/*--------------------------------------------------------------------------*/
void diary(char *str,long int *n,BOOL addCR)
{
#if defined(_MSC_VER)
	/* On Windows, We must have CRLF here */
	char newline[3] = "\r\n";
#else
	char newline[1] = "\n";
#endif

	int u = getdiary();
	if ( u != 0 )
	{
		FILE *fd= GetFileOpenedInScilab(u);
		if (fd)
		{
			if (addCR)
			{
				fprintf(fd,"%s%s",str,newline);
			}
			else
			{
				fprintf(fd,"%s",str);
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
BOOL closeAllDiaries(void)
{
	BOOL bClosed = FALSE;

	/* close previous diary */
	int fd_diary = getdiary();
	if (fd_diary != 0)
	{
		double res = 0.0;
		C2F(mclose)(&fd_diary, &res);
		setDiaryId(0);
		bClosed = TRUE;
	}

	return bClosed;
}
/*--------------------------------------------------------------------------*/
