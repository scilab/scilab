/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "transposeMatrix.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
double *transposeMatrixDouble(int C,int L,double *MatrixDouble)
{
	double *buffer = NULL;
	if (MatrixDouble)
	{
		buffer = (double *)MALLOC( (C*L)*sizeof(double) );
		if (buffer)
		{
			int i = 0;
			int j = 0;

			for (i = 0; i<C; i++) for (j = 0; j < L; j++) 
			{
				buffer[ i*L+j ] = MatrixDouble[ j*C+i ];
			}
		}
	}
	return buffer;
}
/*--------------------------------------------------------------------------*/
int *transposeMatrixInt(int C,int L,int *MatrixInt)
{
	int *buffer = NULL;
	if (MatrixInt)
	{
		buffer = (int *)MALLOC( (C*L)*sizeof(int) );
		if (buffer)
		{
			int i = 0;
			int j = 0;

			for (i = 0; i < C; i++) for (j = 0; j < L; j++) 
			{
				buffer[ i*L+j ] = MatrixInt[ j*C+i ];
			}
		}
	}
	return buffer;
}
/*--------------------------------------------------------------------------*/
/*char **transposeMatrixStrings(int C,int L,char **MatrixStr)
{
	char **buffer = NULL;
	if (MatrixStr)
	{
		buffer = (char **)MALLOC( (C*L)*sizeof(char*) );
		if (buffer)
		{
			int i = 0;
			int j = 0;

			for (i = 0; i < C; i++) for (j = 0; j <  L; j++) 
			{
				buffer[ i*L+j ] = strdup(MatrixStr[ j*C+i ]);
			}
		}
	}
	return buffer;
}
*/
/*--------------------------------------------------------------------------*/
