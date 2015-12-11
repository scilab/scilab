
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

/*--------------------------------------------------------------------------*/
#ifndef __CONVSTR_H__
#define __CONVSTR_H__

#include "dynlib_string.h"

#define LOW 'l'
#define LOW_B 'L'
#define UPPER 'u'
#define UPPER_B 'U'

/**
* convstr case conversion
* param[in] Input_Matrix Input string matrix
* param[out] Output_Matrix
* param[in] typ  'l' or 'u'  means low or upper
* param[in] Number_Input   the number of input string matrix
*/
STRING_IMPEXP void convstr(wchar_t **Input_Matrix, wchar_t **Output_Matrix, char typ, int Number_Input);

#endif /* __CONVSTR_H__ */
/*--------------------------------------------------------------------------*/

