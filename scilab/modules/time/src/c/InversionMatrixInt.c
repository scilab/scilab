
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/* @TODO this file should be moved somewhere more appropriated */
/*--------------------------------------------------------------------------*/
#include "InversionMatrixInt.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int *InversionMatrixInt(int W,int L,int *Matrix)
{
	int *buffer = NULL;
	if (Matrix)
	{
		buffer = (int *)MALLOC( (W*L)*sizeof(int) );
		if (buffer)
		{
			int i=0;
			int j=0;

			for (i=0; i<W; i++) for (j=0; j<L; j++) 
			{
				buffer[ i*L+j ] = Matrix[ j*W+i ];
			}
		}
	}
	return buffer;
}
/*--------------------------------------------------------------------------*/
