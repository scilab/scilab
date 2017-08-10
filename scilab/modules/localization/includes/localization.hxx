/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __LOCALIZATION_HXX__
#define __LOCALIZATION_HXX__

#include <string>

extern "C"
{
#include "dynlib_localization.h"
#include "localization.h"
}

#define _W(String)  gettextW(_(String))

LOCALIZATION_IMPEXP std::wstring gettextW(const char* s);

#endif /* !__LOCALIZATION_HXX__ */