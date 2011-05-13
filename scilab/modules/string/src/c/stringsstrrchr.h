
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2011 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
#ifndef __STRINGSTRRCHAR_H__
#define __STRINGSTRRCHAR_H__

#include "BOOL.h"
#include "charEncoding.h"

/*
* Locate last occurrence of character in string or
* Locate first occurrence of character in string
* @param[in] String matrix One
* @param[in] Dimension m*n of String matrix one
* @param[in] String matrix Two
* @param[in] Dimension m*n of String matrix two
* @param[in] BOOL to do strchr (first occurrence)
* @return a string matrix (result of strchr or strrchr)
*/
wchar_t ** strings_wcsrchr(const wchar_t **InputStrings, int Dim_InputStrings,
                           const wchar_t** InputChar, int Dim_InputChar, BOOL do_strchr);

#endif /* __STRINGSTRRCHAR_H__ */
/*----------------------------------------------------------------------------*/


