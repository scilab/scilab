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
static char* removeBlanks(char *_str)
{
	int len = 0;
	int i = 0;
	len = (int) strlen(_str);
	for(i = len - 1;i >=0 ; i--)
	{
		if (_str[i] != ' ') break;
	}
	_str[i+1] = '\0';
	return  _str;
}
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
void diary(char *str,BOOL addCR)
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
		int len = 0;

		str = removeBlanks(str);
		len = (int) strlen (str);

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
