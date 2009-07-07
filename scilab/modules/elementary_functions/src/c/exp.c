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

double dexps(double _dblVal)
{
	return exp(_dblVal);
}

void zexps(double _dblRealIn, double _dblImgIn, double *_pdblRealOut, double *_pdblImgOut)
{
	*_pdblRealOut	= dexps(_dblRealIn) * dcoss(_dblImgIn);
	*_pdblImgOut	= dexps(_dblRealIn) * dsins(_dblImgIn);
}