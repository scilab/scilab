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
#include "MALLOC.h"
#include "scistrtostr.h"
#include "cvstr.h"
/*--------------------------------------------------------------------------*/
#define MEM_LACK -3
/*--------------------------------------------------------------------------*/
int SciStrtoStr (int *Scistring, int *nstring, int *ptrstrings, char **strh)
{
	char *s = NULL, *p = NULL;
	int li = 0,ni = 0,*SciS = NULL,i = 0,job = 1;

	li = ptrstrings[0];
	ni = ptrstrings[*nstring] - li + *nstring +1;

	p = (char *) MALLOC(ni * sizeof(char) );

	if (p == NULL)  return MEM_LACK;

	SciS = Scistring;
	s = p;
	for ( i = 1 ; i < *nstring + 1 ; i++)
	{
		ni = ptrstrings[i] - li;
		li = ptrstrings[i];
		F2C(cvstr)(&ni,SciS,s,&job,(long int)ni);
		SciS += ni;
		s += ni;
		if (i < *nstring) 
		{
			*s = '\n';
			s++;
		}
	}
	*s = '\0';
	*strh = p;
	return 0;
}
/*--------------------------------------------------------------------------*/
