/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#ifndef __LOCALIZATION_HXX__
#define __LOCALIZATION_HXX__

#include <string>

extern "C"
{
#include "dynlib_localization.h"
#include "charEncoding.h"
#include "localization.h"
#include "sci_malloc.h"
}

#define _W(String)  gettextW(_(String))

LOCALIZATION_IMPEXP std::wstring gettextW(const char* s);

#endif /* !__LOCALIZATION_HXX__ */