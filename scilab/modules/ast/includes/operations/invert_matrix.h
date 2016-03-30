/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bernard HUGUENEY
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

#ifndef _INVERT_MATRIX_H_
#define _INVERT_MATRIX_H_

#include "dynlib_ast.h"

EXTERN_AST int iInvertMatrixM(int iRows, int iCols, double* pData, int complexArg
                              , double* pdblRcond);

#endif /* !_INVERT_MATRIX_H_ */
