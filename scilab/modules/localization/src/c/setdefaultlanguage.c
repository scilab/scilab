/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifdef _MSC_VER
#include "setgetlanguage.h"
#include "LanguagePreferences_Windows.h"
#include "string.h"
#endif

/*--------------------------------------------------------------------------*/
void setdefaultlanguage(char * lang)
{

#ifdef _MSC_VER
    char *savedLanguage = getLanguagePreferences();
    if (strcmp(lang, savedLanguage))
    {
        if (setlanguage(lang))
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
    return getLanguagePreferences();
#else
    return "";
#endif

}
/*--------------------------------------------------------------------------*/
