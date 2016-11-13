/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Antoine ELIAS
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
#include "elem_common.h"
#include "sin.h"
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
