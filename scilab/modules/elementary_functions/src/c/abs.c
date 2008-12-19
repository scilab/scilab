/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <math.h>

#include "elem_common.h"
/*abs*/
double dabss(double _dblVal)
{
	return fabs(_dblVal);
}

/*module of complex*/
double dabsz(double _dblRealVal, double _dblImgVal)
{
	double dblAbsReal	= dabss(_dblRealVal);
	double dblAbsImg	= dabss(_dblImgVal);
	double dblMax		= Max(dblAbsReal, dblAbsImg);
	double dblMin		= Min(dblAbsReal, dblAbsImg);
	if(dblMin == 0)
	{
		return dblMax;
	}
	else
	{
		return dblMax * (sqrt(1 + pow(dblMin / dblMax, 2)));
	}
	/*
	W = MAX( XABS, YABS )
	Z = MIN( XABS, YABS )
	*/

	return 0;
}