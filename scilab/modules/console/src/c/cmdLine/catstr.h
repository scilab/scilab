/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#ifndef	CATSTR_H_
#define	CATSTR_H_

#include <wchar.h>

/**
 * Malloc and concatenate 2 wide char string.
 * @param first wide char string to concatenate with the second one.
 * @param second wide char string to concatenate with the first one.
 * @return Concatenation of both @param.
 */
wchar_t *catStr(wchar_t * str1, wchar_t * str2);

/*
 * Note: returned wide char string will be like that:
 * wcs1wcs2
 * Just like strcat.
 */

#endif /* !CATSTR_H_ */
