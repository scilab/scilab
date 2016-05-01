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
types::Function::ReturnValue sci_covStart(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "covStart" , 1);
        return types::Function::Error;
    }

    if (!in[0]->isMacro() && !in[0]->isMacroFile() && (!in[0]->isString() || (in[0]->getAs<types::String>()->getCols() != 2 && in[0]->getAs<types::String>()->getCols() != 1)))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A two-columns string matrix expected.\n"), "covStart" , 1);
        return types::Function::Error;
    }

    if (!in[0]->isMacro() && !in[0]->isMacroFile())
    {
        types::String * strs = in[0]->getAs<types::String>();
        const unsigned int rows = strs->getRows();

        if (strs->getSize() == 2)
        {
            std::vector<std::pair<std::wstring, std::wstring>> paths_mods;
            paths_mods.reserve(rows);

            for (unsigned int i = 0; i < rows; ++i)
            {
                paths_mods.emplace_back(strs->get(i, 0), strs->get(i, 1));
            }

            coverage::CoverModule::createInstance(paths_mods);
        }
        else
        {
            std::vector<std::wstring> mods;
            mods.reserve(rows);

            for (unsigned int i = 0; i < rows; ++i)
            {
                mods.emplace_back(strs->get(i, 0));
            }

            coverage::CoverModule::createInstance(mods);
        }
    }
    else
    {
        if (in[0]->isMacro())
        {
            coverage::CoverModule::createInstance()->instrumentSingleMacro(L"", L"", static_cast<types::Macro *>(in[0]), false);
        }
        else
        {
            coverage::CoverModule::createInstance()->instrumentSingleMacro(L"", L"", static_cast<types::MacroFile *>(in[0])->getMacro(), false);
        }
    }

    return types::Function::OK;
}
