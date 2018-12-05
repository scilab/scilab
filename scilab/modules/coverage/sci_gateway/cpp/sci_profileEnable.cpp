/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2018 - ESI Group - Clement DAVID
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
using namespace coverage;
/*--------------------------------------------------------------------------*/
namespace
{
void addToProcess(CoverModule* cm, types::Macro* current)
{
    cm->instrumentSingleMacro(current->getModule(), current->getFileName(), current, true);
};
} /* namespace */
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_profileEnable(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	CoverModule* cm = CoverModule::getInstance();
    if (cm == nullptr)
    {
        cm = CoverModule::createInstance();
        if (cm == nullptr)
        {
			Scierror(999, _("%s: No more memory.\n"), "profileEnable");
			return types::Function::ReturnValue::Error;
		}
    }

	// instrument all
	if (in.empty())
	{
        std::list<std::wstring> lst;
        symbol::Context::getInstance()->getMacrosName(lst);

		for (const std::wstring& macroName : lst)
		{
            types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(macroName));
            if (pIT->isMacro())
			{
				addToProcess(cm, pIT->getAs<types::Macro>());
			}
			else if (pIT->isMacroFile())
			{
				types::MacroFile* macroFile = pIT->getAs<types::MacroFile>();
				addToProcess(cm, macroFile->getMacro());
			}
		}
	}

    // handle macros arguments (stored into Library, MacroFile or self)
    for (size_t idx = 0; idx < in.size(); idx++)
    {
        types::InternalType* pIT = in[idx];

        // case Library
        if (pIT->isLibrary())
        {
            types::Library* lib = pIT->getAs<types::Library>();
            std::list<std::wstring> lst;
            int sz = lib->getMacrosName(lst);
            if (sz > 0)
            {
                for (const std::wstring& macro : lst)
                {
                    types::MacroFile* macroFile = lib->get(macro);
                    addToProcess(cm, macroFile->getMacro());
                }
                continue;
            }
        }

        // case MacroFile
        if (pIT->isMacroFile())
        {
            types::MacroFile* macroFile = pIT->getAs<types::MacroFile>();
            addToProcess(cm, macroFile->getMacro());
            continue;
        }

        // case Macro
        if (pIT->isMacro())
        {
            addToProcess(cm, pIT->getAs<types::Macro>());
            continue;
        }

        // default
        Scierror(999, _("%s: Wrong type for input argument #%d: A macro or library expected.\n"), "profileGetInfo", idx + 1);
        return types::Function::ReturnValue::Error;
    }

    return types::Function::OK;
}
