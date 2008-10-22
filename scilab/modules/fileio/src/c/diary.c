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
/*--------------------------------------------------------------------------*/
int getdiary(void)
{
	return C2F(iop).wio;
}
/*--------------------------------------------------------------------------*/
void diary(char *str,long int *n)
{
#if defined(_MSC_VER)
	/* On Windows, We must have CRLF here */
	char newline[3]="\r\n";
	int nn=2;
#else
	char newline[1]="\n";
	int nn=1;
#endif
	int u;

	u=getdiary();
	if ( u != 0 )
	{
		FILE *fd= GetFileOpenedInScilab(u);
		fwrite(str,sizeof(unsigned char),*n,fd);
		fwrite(newline,sizeof(unsigned char),nn,fd);
	}
}
/*--------------------------------------------------------------------------*/
void diary_nnl(char *str,int *n)
{
	int u;

	u=getdiary();

	if (u)
	{
		FILE *fd= GetFileOpenedInScilab(u);
		fwrite(str,sizeof(unsigned char),*n,fd);
	}
}
/*--------------------------------------------------------------------------*/
