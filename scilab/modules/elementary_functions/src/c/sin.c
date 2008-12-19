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
#include "elem_common.h"

/*sin*/
double dsins(double _dblVal)
{
	return sin(_dblVal);
}

void zsins(double _dblRealIn, double _dblImgIn, double* _dblRealOut, double* _dblImgOut)
{
	*_dblRealOut	= dsins(_dblRealIn) * dcoshs(_dblImgIn);
	*_dblImgOut		= dcoss(_dblRealIn) * dsinhs(_dblImgIn);
}
