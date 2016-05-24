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

extern "C"
{
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "defaultlanguage.h"
#include "configvariable_interface.h"
#include "setgetlanguage.h"
}

types::Function::ReturnValue sci_setlanguage(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "setlanguage", 1);
        return types::Function::Error;
    }

    if (_piRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "setlanguage", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "setlanguage", 1);
        return types::Function::Error;
    }

    wchar_t *param = in[0]->getAs<types::String>()->get(0);
    const wchar_t *newlanguage = convertlanguagealias(param);

    if (!LanguageIsOK(param) && (newlanguage == NULL))
    {
        if (getWarningMode())
        {
            sciprint(_("%ls: Unsupported language '%ls'.\n"), L"setlanguage", param);
        }

        out.push_back(new types::Bool(FALSE));
        return types::Function::OK;
    }

    if (newlanguage)
    {
        if (needtochangelanguage(newlanguage))
        {
            if (!setlanguage(newlanguage))
            {
                out.push_back(new types::Bool(FALSE));
            }
            else
            {
                out.push_back(new types::Bool(TRUE));
            }
        }
        else
        {
            out.push_back(new types::Bool(TRUE));
        }
    }
    else
    {
        if (getWarningMode())
        {
            sciprint(_("Unsupported language '%ls'.\n"), param);
            sciprint(_("Switching to default language : '%ls'.\n"), SCILABDEFAULTLANGUAGE);
        }
        setlanguage(SCILABDEFAULTLANGUAGE);
        out.push_back(new types::Bool(FALSE));
    }

    return types::Function::OK;
}
