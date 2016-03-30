/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "localization_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"

#include "scilabWrite.hxx"

extern "C"
{
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "setgetlanguage.h"
#include "os_string.h"
#include "configvariable_interface.h"
#ifdef _MSC_VER
#include "LanguagePreferences_Windows.h"
#endif
}

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static wchar_t *getLanguageFromAlias(wchar_t *alias)
{
    if (alias)
    {
        if ( wcscmp(alias, L"en") == 0 )
        {
            return os_wcsdup(L"en_US");
        }

        if ( wcscmp(alias, L"fr") == 0 )
        {
            return os_wcsdup(L"fr_FR");
        }

        return os_wcsdup(alias);
    }

    /* "" value fixed by system */
    return os_wcsdup(L"");
}
#endif
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_setdefaultlanguage(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "setdefaultlanguage", 1);
        return types::Function::Error;
    }

    if (_piRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "setdefaultlanguage", 1);
        return types::Function::Error;
    }

#ifndef _MSC_VER
    /*
    ** No need to set default language except under Windows.
    ** Will return FALSE
    */
    if (getWarningMode())
    {
        sciprint(_("%ls: This feature is only supported on Windows.\n"), L"setdefaultlanguage");
    }

    types::Bool* pbOut = new types::Bool(FALSE);
    out.push_back(pbOut);
    return types::Function::OK;
#else
    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "setdefaultlanguage" , 1);
        return types::Function::Error;
    }
    wchar_t *newlang = getLanguageFromAlias(in[0]->getAs<types::String>()->get(0));

    if ( !isValidLanguage(newlang) )
    {
        if ( getWarningMode() )
        {
            sciprint(_("Unsupported language '%ls'.\n"), newlang);
        }
        out.push_back(new types::Bool(FALSE));

        return types::Function::OK;
    }
    else
    {
        wchar_t *savedLanguage = getLanguagePreferences();
        if ( wcscmp(newlang, savedLanguage) == 0 )
        {
            /* do nothing */
            out.push_back(new types::Bool(TRUE));

            return types::Function::OK;
        }
        else
        {
            // ??                if (savedLanguage) { FREE(savedLanguage); savedLanguage = NULL; }
            if ( !setlanguage(newlang) ) /* */
            {
                out.push_back(new types::Bool(FALSE));
                return types::Function::OK;
            }
            else
            {
                if ( getWarningMode() )
                {
                    sciprint("\n");
                    sciprint(_("The language for menus cannot be changed on a running console.\n"));
                    sciprint(_("Restart Scilab to apply to menus.\n"));
                }
                if ( setLanguagePreferences() )
                {
                    out.push_back(new types::Bool(TRUE));
                }
                else
                {
                    out.push_back(new types::Bool(FALSE));
                }
                return types::Function::OK;
            }
        }
    }

#endif
}
