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

#include		<unistd.h>
#include		<stdlib.h>
#include		<wchar.h>
#include		<wctype.h>
#include                "MALLOC.h"

/* Concatenate 2 wide char string then return it */
wchar_t *catStr(wchar_t * _wcs1, wchar_t * _wcs2)
{
    wchar_t *catWcs = NULL;
    int sizeOfCatWcs = (1 + wcslen(_wcs1) + wcslen(_wcs2));

    catWcs = MALLOC(sizeof(*catWcs) * sizeOfCatWcs);
    catWcs = wcscpy(catWcs, _wcs1);
    catWcs = wcscat(catWcs, _wcs2);
    catWcs[sizeOfCatWcs] = L'\0';
    return (catWcs);
}
