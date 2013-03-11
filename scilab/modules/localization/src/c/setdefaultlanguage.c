/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifdef _MSC_VER
#include "setgetlanguage.h"
#include "LanguagePreferences_Windows.h"
#include "string.h"
#include "charEncoding.h"
#endif

/*--------------------------------------------------------------------------*/
void setdefaultlanguage(char * lang)
{

#ifdef _MSC_VER
    wchar_t *savedLanguage = getLanguagePreferences();
    wchar_t* pwstLang = to_wide_string(lang);
    if (wcscmp(pwstLang, savedLanguage))
    {
        if (setlanguage(pwstLang))
        {
            setLanguagePreferences();
        }
    }
#endif

}
/*--------------------------------------------------------------------------*/
char * getdefaultlanguage(void)
{

#ifdef _MSC_VER
    return wide_string_to_UTF8(getLanguagePreferences());
#else
    return "";
#endif

}
/*--------------------------------------------------------------------------*/
