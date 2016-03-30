/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009
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

#ifndef __STRINGCONVERT_H__
#define __STRINGCONVERT_H__

#include "dynlib_string.h"
#include <wchar.h>

/**
* changes `\``n` --> `\n` idem for \t and \r
* @param[in/out] string to convert
* @return number of \n found
*/
STRING_IMPEXP int StringConvert(char *str);
STRING_IMPEXP int StringConvertW(wchar_t *str);

#endif /* __STRINGCONVERT_H__ */
