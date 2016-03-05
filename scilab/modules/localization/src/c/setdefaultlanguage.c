/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifdef _MSC_VER
#include "setgetlanguage.h"
#include "LanguagePreferences_Windows.h"
#include "string.h"
#include "charEncoding.h"
#endif

/*--------------------------------------------------------------------------*/
void setdefaultlanguage(const char * lang)
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
const char* getdefaultlanguage(void)
{

#ifdef _MSC_VER
    return wide_string_to_UTF8(getLanguagePreferences());
#else
    return "";
#endif

}
/*--------------------------------------------------------------------------*/
