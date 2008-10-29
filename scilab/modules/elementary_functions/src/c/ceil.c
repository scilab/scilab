/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "basic_functions.h"


/*up round*/
double dceils(double _dblVal)
{
	/*
	3.14 -> 3
	4,00 -> 4

	-3.14 -> -3
	-4.00 -> -4
	*/
	return ceil(_dblVal);
}

/*up round with precision*/
double dceilsEx(double _dblVal, int _iPrecision)
{
	double iPow = pow(10, _iPrecision);

	double dblTemp = _dblVal * iPow;
	dblTemp = dceils(dblTemp);

	return (double)dblTemp / iPow;

}
