/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __ISASCII_H__
#define __ISASCII_H__

#include <wchar.h>
#include "BOOL.h"

#ifndef _MSC_VER
#define iswascii(x) isascii(wctob(x))
#endif

#include "dynlib_string.h"

STRING_IMPEXP BOOL* isasciiStringW(wchar_t* input_string, int* returnedSize);
STRING_IMPEXP BOOL* isasciiMatrix(double* input_string, int matrixSize);

#endif /* __ISASCII_H__ */
/*--------------------------------------------------------------------------*/
