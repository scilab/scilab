/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

using namespace types;

static BOOL loadedDep = FALSE;

Function::ReturnValue sci_helpbrowser(typed_list &in, int _iRetCount, typed_list& out)
{
    int iHelpAdrSize    = 0;
    char **helpAdr      = NULL;
    char **languageAdr  = NULL;

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s:  Wrong number of output argument(s): %d to %d expected."), "helpbrowser", 0, 1);
        return Function::Error;
    }
    switch (in.size())
    {
        case 4:
            if ( !(in[3]->isBool() == true && in[3]->getAs<Bool>()->isScalar() == true))
            {
                Scierror(999, _("%s:  Wrong type for input argument #%d: A boolean expected."), "helpbrowser", 4);
                return Function::Error;
            }
            if ( !(in[2]->isString() == true && in[2]->getAs<String>()->isScalar() == true))
            {
                Scierror(999, _("%s:  Wrong type for input argument #%d: A string expected."), "helpbrowser", 3);
                return Function::Error;
            }
        case 2:
            // Second argument must be String or at least [].
            if ( !(in[1]->isString() == true && in[1]->getAs<String>()->isScalar() == true))
            {
                Scierror(999, _("%s:  Wrong type for input argument #%d: A string expected."), "helpbrowser", 2);
                return Function::Error;
            }
            // Matrix of String or [] allowed.
            if ( !( (in[0]->isString() == true)
                    || (in[0]->isDouble() == true && in[0]->getAs<Double>()->isEmpty() == true)) )
            {
                Scierror(999, _("%s:  Wrong type for input argument #%d: A string expected."), "helpbrowser", 1);
                return Function::Error;
            }
            break;
        default:
            Scierror(77, _("%s:  Wrong number of input argument(s): %d to %d expected."), "helpbrowser", 2, 4);
            return Function::Error;
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
        String *pInHelpAdr =  in[0]->getAs<String>();
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
        pstLang = wide_string_to_UTF8(in[1]->getAs<String>()->get(0));
        launchHelpBrowser(helpAdr, iHelpAdrSize, pstLang);
    }

    if (in.size() == 4)
    {
        pstLang = wide_string_to_UTF8(in[2]->getAs<String>()->get(0));
        pstKey  = wide_string_to_UTF8(in[1]->getAs<String>()->get(0));
        int iFullText = in[3]->getAs<Bool>()->get(0);
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

    return Function::OK;
}
