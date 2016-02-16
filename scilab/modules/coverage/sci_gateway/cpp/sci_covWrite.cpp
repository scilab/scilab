/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
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

#include <string.h>

#include "CoverModule.hxx"

#include "coverage_gw.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "configvariable.hxx"
#include "context.hxx"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_covWrite(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2 && in.size() != 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "covWrite" , 2, 3);
        return types::Function::Error;
    }

    if (!in[0]->isString() || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), "covWrite" , 1);
        return types::Function::Error;
    }

    if (!in[1]->isString() || in[1]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), "covWrite" , 2);
        return types::Function::Error;
    }

    if (in.size() == 2)
    {
        if (coverage::CoverModule * cm = coverage::CoverModule::getInstance())
        {
            // we write the current CoverModule instance
            const std::wstring type(in[0]->getAs<types::String>()->get(0));
            if (type == L"binary")
            {
                cm->collect();
                cm->save(in[1]->getAs<types::String>()->get(0));
            }
            else if (type == L"html")
            {
                cm->collect();
                cm->toHTML(in[1]->getAs<types::String>()->get(0));
            }
        }
        else
        {
            Scierror(999, _("%s: No active coverage: can\'t write.\n"), "covWrite");
            return types::Function::Error;
        }
    }
    else if (in.size() == 3)
    {
        if (!in[2]->isString() || in[2]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), "covWrite" , 3);
            return types::Function::Error;
        }
        const std::wstring type(in[0]->getAs<types::String>()->get(0));
        if (type != L"html")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: html output only.\n"), "covWrite" , 1);
            return types::Function::Error;
        }
        coverage::CoverModule::toHTML(in[1]->getAs<types::String>()->get(0), in[2]->getAs<types::String>()->get(0));
    }

    return types::Function::OK;
}
