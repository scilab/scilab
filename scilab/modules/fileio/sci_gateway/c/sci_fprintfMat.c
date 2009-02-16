/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "MALLOC.h"
#include "stack-c.h"
#include "gw_fileio.h"
#include "fileio.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
int int_objfprintfMat(char *fname,unsigned long fname_len)
{
	int l1 = 0, m1 = 0, n1 = 0,l2 = 0,m2 = 0,n2 = 0,m3 = 0,n3 = 0,l3 = 0,i = 0,j = 0,mS = 0,nS = 0;
	FILE  *f;
	char *filename = NULL;
	char **Str2 = NULL;
	char *Format = NULL;
	Nbvars = 0;
	CheckRhs(1,4);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char szTemp[bsiz];
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);/* file name */

		/* BUG 3714 */
		filename = UTFToLocale(cstk(l1), szTemp);
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	if (GetType(2) == sci_matrix)
	{
		#define COMPLEXPART 1
		int *header=NULL;
		int Cmplx;

		header = (int *) GetData(2);
		Cmplx=header[3];

		if (Cmplx != COMPLEXPART)
		{
			GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2); /* data */
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Real values expected.\n"),fname,2);
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of floating point numbers expected.\n"),fname,2);
		return 0;
	}

	if ( Rhs >= 3)
	{
		GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);/* format */
		StringConvert(cstk(l3));  /* conversion */
		Format = cstk(l3);
	}
	else
	{
		Format = "%f";
	}

	if ( Rhs >= 4 )
	{
		GetRhsVar(4,MATRIX_OF_STRING_DATATYPE,&mS,&nS,&Str2);
	}

	if (( f = fopen(filename,"w")) == (FILE *)0)
	{
		Scierror(999,_("%s: Cannot open file %s.\n"),fname,filename);
		return 0;
	}

	if ( Rhs >= 4 )
	{
		char szTempLocale[bsiz];
		for ( i=0 ; i < mS*nS ; i++) fprintf(f,"%s\n",UTFToLocale(Str2[i], szTempLocale));
	}

	for (i = 0 ; i < m2 ; i++ )
	{
		for ( j = 0 ; j < n2 ; j++)
		{
			fprintf(f,Format,*stk(l2+i + m2*j));
			fprintf(f," ");
		}

		fprintf(f,"\n");
	}
	fclose(f);
	LhsVar(1)=0 ; /** no return value **/
	if ( Rhs >= 4)
	{
		freeArrayOfString(Str2,mS*nS);
	}
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
