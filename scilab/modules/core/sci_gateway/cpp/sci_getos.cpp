/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

#include "core_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "string.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "getos.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_getos(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    char *OperatingSystem = getOSFullName();

    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "getos", 0);
        FREE(OperatingSystem);
        OperatingSystem = NULL;
        return types::Function::Error;
    }

    if (_iRetCount != 2 && _iRetCount != 1 && _iRetCount != -1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "getos", 1, 2);
        FREE(OperatingSystem);
        OperatingSystem = NULL;
        return types::Function::Error;
    }

    if (OperatingSystem)
    {
        types::String* pS1 = new types::String(OperatingSystem);
        out.push_back(pS1);
        if (OperatingSystem)
        {
            FREE(OperatingSystem);
            OperatingSystem = NULL;
        }


        if (_iRetCount == 2)
        {
            char *Release = getOSRelease();
            if (Release)
            {
                types::String* pS2 = new types::String(Release);
                out.push_back(pS2);
                if (Release)
                {
                    FREE(Release);
                    Release = NULL;
                }
            }
            else
            {
                Scierror(999, _("%s: No more memory.\n"), "getos");
                return types::Function::Error;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: No more memory.\n"), "getos");
        return types::Function::Error;
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
