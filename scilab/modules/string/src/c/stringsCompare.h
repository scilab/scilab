
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#ifndef __STRINGSCOMPARE_H__
#define __STRINGSCOMPARE_H__

#include "BOOL.h"
#include <wchar.h>
#include "dynlib_string.h"

/**
* Compare strings matrix
* @param[in] String matrix One
* @param[in] Dimension m*n of String matrix one
* @param[in] String matrix Two
* @param[in] Dimension m*n of String matrix two
* @param[in] BOOL to do stricmp
* @return a int matrix (result of strcmp)
*/
STRING_IMPEXP int * stringsCompare(wchar_t **Input_String_One, int dim_One, wchar_t **Input_String_Two, int dim_Two, BOOL dostricmp);

#endif /* __STRINGSCOMPARE_H__ */
/*--------------------------------------------------------------------------*/
