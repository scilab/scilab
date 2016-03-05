/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "transposeMatrix.h"
#include "os_string.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
double *transposeMatrixDouble(int C, int L, double *MatrixDouble)
{
    double *buffer = NULL;
    if (MatrixDouble)
    {
        buffer = (double *)MALLOC( (C * L) * sizeof(double) );
        if (buffer)
        {
            int i = 0;
            int j = 0;

            for (i = 0; i < C; i++) for (j = 0; j < L; j++)
                {
                    buffer[ i * L + j ] = MatrixDouble[ j * C + i ];
                }
        }
    }
    return buffer;
}
/*--------------------------------------------------------------------------*/
int *transposeMatrixInt(int C, int L, int *MatrixInt)
{
    int *buffer = NULL;
    if (MatrixInt)
    {
        buffer = (int *)MALLOC( (C * L) * sizeof(int) );
        if (buffer)
        {
            int i = 0;
            int j = 0;

            for (i = 0; i < C; i++) for (j = 0; j < L; j++)
                {
                    buffer[ i * L + j ] = MatrixInt[ j * C + i ];
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
				buffer[ i*L+j ] = os_strdup(MatrixStr[ j*C+i ]);
			}
		}
	}
	return buffer;
}
*/
/*--------------------------------------------------------------------------*/
