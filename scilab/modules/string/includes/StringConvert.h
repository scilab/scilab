/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#ifndef __STRINGCONVERT_H__
#define __STRINGCONVERT_H__

#include "dynlib_string.h"

/**
* changes `\``n` --> `\n` idem for \t and \r
* @param[in/out] string to convert
* @return number of \n found
*/
STRING_IMPEXP int StringConvert(char *str);

#endif /* __STRINGCONVERT_H__ */
