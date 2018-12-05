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

#include "configvariable.hxx"
#include "context.hxx"
#include "coverage_gw.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"

#include <fstream>
#include <iostream>
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

void addToProcess(CoverModule* cm, std::vector<types::Macro*>& macros, types::Macro* current)
{
    size_t macroLen = macros.size();
    macros.push_back(current);

    // clean sub functions (defined in the same file)
    std::vector<types::Macro*> macrosStack{current};
    for (types::Macro* m = macrosStack.back(); !macrosStack.empty(); m = macrosStack.back(), macrosStack.pop_back())
    {
        for (const auto& p : m->getSubMacros())
        {
            macros.push_back(p.second);
            macrosStack.push_back(p.second);
        }
    }
}

} /* namespace */
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_profileDisable(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
    CoverModule* cm = CoverModule::getInstance();
	if (cm == nullptr && in.size() == 0)
	{
		// nothing to do, OK
		return types::Function::ReturnValue::OK;
	}

    if (cm == nullptr)
    {
        Scierror(999, _("%s: profile is disabled.\n"), "profileDisable");
        return types::Function::ReturnValue::Error;
    }

    // store macros arguments (stored into Library, MacroFile or self)
    std::vector<types::Macro*> macrosToClean;
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
                const std::wstring& selectedModule(lib->get(lst.front())->getModule());
                for (const std::wstring& macro : lst)
                {
                    types::MacroFile* macroFile = lib->get(macro);
                    addToProcess(cm, macrosToClean, macroFile->getMacro());
                }
                continue;
            }
        }

        // case MacroFile
        if (pIT->isMacroFile())
        {
            types::MacroFile* macroFile = pIT->getAs<types::MacroFile>();
            addToProcess(cm, macrosToClean, macroFile->getMacro());
            continue;
        }

        // case Macro
        if (pIT->isMacro())
        {
            addToProcess(cm, macrosToClean, pIT->getAs<types::Macro>());
            continue;
        }

        // default
        Scierror(999, _("%s: Wrong type for input argument #%d: A macro or library expected.\n"), "profileGetInfo", idx + 1);
        return types::Function::ReturnValue::Error;
    }

    // Full cleanup
    if (macrosToClean.empty())
    {
        coverage::CoverModule::clearInstance();
        return types::Function::ReturnValue::OK;
    }

    // Clean only the selected macros and their inners/nested function
    std::vector<Counter> counters(cm->getCounters());
    std::sort(counters.begin(), counters.end(), CounterPredicate::by_file_and_location());

    for (types::Macro* m : macrosToClean)
    {
        std::vector<Counter>::iterator found = std::lower_bound(counters.begin(), counters.end(), m, CounterPredicate::by_file_and_location());
        if (found == counters.end())
        {
            continue;
        }

        while (found != counters.end() && found->getExp()->getLocation().last_line <= m->getBody()->getLocation().last_line)
        {
            found->getExp()->setCoverId(0);

            found++;
        }

        cm->getCallCounters().erase(m);
        m->DecreaseRef();

        // FIXME cleanup previously collected CoverModule maps ?
    }

    // compact the retained Counters
    std::vector<Counter>& refCounters = cm->getCounters();
    uint64_t removed = 0;
    std::vector<Counter>::iterator erased = std::remove_if(refCounters.begin(), refCounters.end(), [&](Counter& c) -> bool {
        if (c.getExp()->getCoverId() == 0)
        {
            removed++;
            return true;
        }
        else
        {
            c.getExp()->setCoverId(c.getExp()->getCoverId() - removed);
            return false;
        }
    });
    refCounters.erase(erased, refCounters.end());

    return types::Function::OK;
}
