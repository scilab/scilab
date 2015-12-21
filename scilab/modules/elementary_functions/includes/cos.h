/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __COS_H__
#define __COS_H__

#include "dynlib_elementary_functions.h"

ELEMENTARY_FUNCTIONS_IMPEXP double dcoss(double _dblVal);
ELEMENTARY_FUNCTIONS_IMPEXP void zcoss(double _dblRealIn, double _dblImgIn, double* _dblRealOut, double* _dblImgOut);

ELEMENTARY_FUNCTIONS_IMPEXP double dcoshs(double _dblVal);

#endif /* __COS_H__ */
