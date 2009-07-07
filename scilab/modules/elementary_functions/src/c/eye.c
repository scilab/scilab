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

void deyes(double *_pdblOut, int _iRows, int _iCols)
{
	vDset(_iRows * _iCols, 0, _pdblOut, 1);
	vDset(Min(_iRows, _iCols), 1, _pdblOut, _iRows + 1);
}
