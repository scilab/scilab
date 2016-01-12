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

#include "core_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include "configvariable_interface.h"
#include "charEncoding.h"
#include "sciprint.h"
#include "getos.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "os_string.h"
#include "Sciwarning.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_warning(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String *psInput = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "warning", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "warning", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "warning", 1);
        return types::Function::Error;
    }

    psInput = in[0]->getAs<types::String>();

    if (psInput->getSize() == 1)
    {
        /* "on" "off" "query" "stop" */
        if (strcmp(psInput->get(0), "on") == 0)
        {
            setWarningMode(TRUE);
            setWarningStop(FALSE);
            return types::Function::OK;
        }

        if (strcmp(psInput->get(0), "off") == 0)
        {
            setWarningMode(FALSE);
            setWarningStop(FALSE);
            return types::Function::OK;
        }

        if (strcmp(psInput->get(0), "stop") == 0)
        {
            setWarningMode(TRUE);
            setWarningStop(TRUE);
            return types::Function::OK;
        }

        if (strcmp(psInput->get(0), "query") == 0)
        {
            if (getWarningMode())
            {
                if (getWarningStop())
                {
                    // WarningMode and WarningStop => warning stop mode active
                    out.push_back(new types::String("stop"));
                }
                else
                {
                    // WarningMode and !WarningStop => warning on  mode active
                    out.push_back(new types::String("on"));
                }
            }
            else
            {
                out.push_back(new types::String("off"));
            }
            return types::Function::OK;
        }
    }

    if (getWarningMode())
    {
        for (int i = 0; i < psInput->getSize() ; ++i)
        {
            char* pstTemp = psInput->get(i);
            size_t iSize = (strlen(pstTemp) + 10 + 1);
            char* pstToPrint = (char*)MALLOC(sizeof(char) * iSize);
            os_sprintf(pstToPrint, iSize, _("WARNING: %s\n"), pstTemp);
            scilabForcedWrite(pstToPrint);
            FREE(pstToPrint);
        }

        if (getWarningStop())
        {
            Sciwarning("");
        }
    }


    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
