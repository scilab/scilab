/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - Scilab Enterprises - Cedric Delamarre
 *
 * Copyright (C) 2012 - 2017 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __OS_WCSTOD_H__
#define __OS_WCSTOD_H__

#include <wchar.h>
#include "dynlib_string.h"

// because of string 'd' and 'D' have been removed from the convertion of wcstod,
// this function will allow the convertion of strings which use theses characteres
// to keep compatibility
STRING_IMPEXP double os_wcstod(wchar_t *_pwstData, wchar_t** _pswtState);

#endif /* !__OS_WCSTOD_H__ */
