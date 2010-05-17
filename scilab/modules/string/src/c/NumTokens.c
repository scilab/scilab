/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif

#include <string.h>
#include <stdio.h>
#include "NumTokens.h"
/*--------------------------------------------------------------------------*/
int NumTokens(char *string)
{
	if (string)
	{
		char buf[128];
		int n      = 1;
		int lnchar = 0;
		int ntok   = -1;
		int length = (int)strlen(string)+1;

		if (string != 0)
		/** Counting leading white spaces **/
		sscanf(string,"%*[ \r\t\n]%n",&lnchar);

		while ( n != 0 && n != EOF && lnchar <= length  )
		{
			int nchar1=0,nchar2=0;
			ntok++;
			n       = sscanf(&(string[lnchar]),"%[^ \r\t\n]%n%*[ \r\t\n]%n",buf,&nchar1,&nchar2);
			lnchar += (nchar2 <= nchar1) ? nchar1 : nchar2 ;
		}

		return(ntok);
	}
	return(1);
}
/*--------------------------------------------------------------------------*/
