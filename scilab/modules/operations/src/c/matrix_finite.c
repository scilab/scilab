/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "finite.h"
#include "core_math.h"

/*return 1 if all is OK else 0*/
int matrix_finite(double *_pdbl, int _iSize)
{
	int i = 0;
	for(i = 0 ; i < _iSize ; i++)
	{
		if(finite(_pdbl[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}
