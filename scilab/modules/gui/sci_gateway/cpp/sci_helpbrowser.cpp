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

#include "gui_gw.hxx"
#include "function.hxx"
#include "bool.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "loadOnUseClassPath.h"
#include "CallHelpBrowser.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

static BOOL loadedDep = FALSE;

types::Function::ReturnValue sci_helpbrowser(types::typed_list &in, int _iRetCount, types::typed_list& out)
{
    int iHelpAdrSize    = 0;
    char **helpAdr      = NULL;
    char **languageAdr  = NULL;

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s:  Wrong number of output argument(s): %d to %d expected."), "helpbrowser", 0, 1);
        return types::Function::Error;
    }
    switch (in.size())
    {
        case 4:
            if (!(in[3]->isBool() == true && in[3]->getAs<types::Bool>()->isScalar() == true))
            {
                Scierror(999, _("%s:  Wrong type for input argument #%d: A boolean expected."), "helpbrowser", 4);
                return types::Function::Error;
            }
            if (!(in[2]->isString() == true && in[2]->getAs<types::String>()->isScalar() == true))
            {
                Scierror(999, _("%s:  Wrong type for input argument #%d: string expected."), "helpbrowser", 3);
                return types::Function::Error;
            }
        case 2:
            // Second argument must be String or at least [].
            if (!(in[1]->isString() == true && in[1]->getAs<types::String>()->isScalar() == true))
            {
                Scierror(999, _("%s:  Wrong type for input argument #%d: string expected."), "helpbrowser", 2);
                return types::Function::Error;
            }
            // Matrix of String or [] allowed.
            if ( !( (in[0]->isString() == true)
                    || (in[0]->isDouble() == true && in[0]->getAs<types::Double>()->isEmpty() == true)))
            {
                Scierror(999, _("%s:  Wrong type for input argument #%d: string expected."), "helpbrowser", 1);
                return types::Function::Error;
            }
            break;
        default:
            Scierror(77, _("%s:  Wrong number of input argument(s): %d to %d expected."), "helpbrowser", 2, 4);
            return types::Function::Error;
    }

    /* We load SciNotes when calling javahelp because we have no way to know
     * to load it when using Javahelp because it can call SciNotes directly */
    if (!loadedDep)
    {
        loadOnUseClassPath("SciNotes");
        loadedDep = TRUE;
    }

    if (in[0]->isString() == true)
    {
        types::String *pInHelpAdr = in[0]->getAs<types::String>();
        helpAdr = new char*[pInHelpAdr->getSize()];
        iHelpAdrSize = pInHelpAdr->getSize();

        for (int i = 0 ; i < pInHelpAdr->getSize() ; ++i)
        {
            helpAdr[i] = wide_string_to_UTF8(pInHelpAdr->get(i));
        }
    }

    char* pstLang   = NULL;
    char* pstKey    = NULL;
    if (in.size() == 2)
    {
        pstLang = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
        launchHelpBrowser(helpAdr, iHelpAdrSize, pstLang);
    }

    if (in.size() == 4)
    {
        pstLang = wide_string_to_UTF8(in[2]->getAs<types::String>()->get(0));
        pstKey = wide_string_to_UTF8(in[1]->getAs<types::String>()->get(0));
        int iFullText = in[3]->getAs<types::Bool>()->get(0);
        searchKeyword(helpAdr, iHelpAdrSize, pstKey, pstLang, (BOOL) iFullText);
    }

    if (pstLang != NULL)
    {
        FREE(pstLang);
    }

    if (pstKey != NULL)
    {
        FREE(pstKey);
    }

    if (helpAdr != NULL) /* No toolboxes loaded */
    {
        for (int i = 0 ; i < iHelpAdrSize ; i++)
        {
            FREE(helpAdr[i]);
        }
        delete[] helpAdr;
    }

    return types::Function::OK;
}
