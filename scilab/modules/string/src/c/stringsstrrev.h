
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#ifndef __STRINGSSTRREV_H__
#define __STRINGSSTRREV_H__

#include <wchar.h>
#include "dynlib_string.h"

STRING_IMPEXP wchar_t **strings_strrev(wchar_t **Input_strings, int Dim_Input_strings);
wchar_t* scistrrev(wchar_t* str);

#endif /* __STRINGSSTRREV_H__ */
/*----------------------------------------------------------------------------*/
