/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
