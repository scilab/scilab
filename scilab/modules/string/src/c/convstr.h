
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

