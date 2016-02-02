/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO -  Antoine ELIAS
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

#include "os_string.h"

wchar_t *_sciwcsdup(const wchar_t *_pwcsSource)
{
    wchar_t * pwcsDest = (wchar_t *) malloc(sizeof(wchar_t) * (wcslen(_pwcsSource) + 1));
    wcscpy(pwcsDest, _pwcsSource);

    return pwcsDest;
}
