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
#include "double.hxx"
#include "int.hxx"
#include "scilabWrite.hxx"
#include "scilabexception.hxx"
#include "string.hxx"
#include "struct.hxx"

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

bool is_nested_function(const std::map<std::wstring, std::pair<int, int>>& fileDescription, const std::map<std::wstring, std::pair<int, int>>::const_iterator& found, types::Macro* macro)
{
    if (found != fileDescription.end() && found->first == macro->getFileName())
    {
        return macro->getLastLine() < found->second.second;
    }
    return false;
}

bool is_sub_function(const std::map<std::wstring, std::pair<int, int>>& fileDescription, const std::map<std::wstring, std::pair<int, int>>::const_iterator& found, types::Macro* macro)
{
    if (found != fileDescription.end() && found->first == macro->getFileName())
    {
        return macro->getLastLine() > found->second.second;
    }
    return false;
}

void populateFunctionTable(types::Struct* functionTable, const std::map<std::wstring, std::pair<int, int>>& fileDescription, types::Macro* macro)
{
    // first call
    if (functionTable->getSize() == 0)
    {
        types::SingleStruct* functionTableStruct = new types::SingleStruct();

        int i = 0;
        auto& fields = functionTableStruct->getFields();
        auto& data = functionTableStruct->getData();

        fields.insert({L"FunctionName", i++});
        data.push_back(new types::String(macro->getName().c_str()));

        fields.insert({L"FileName", i++});
        data.push_back(new types::String(macro->getFileName().c_str()));

        fields.insert({L"FirstLine", i++});
        data.push_back(new types::Double(macro->getFirstLine()));

        fields.insert({L"LibraryName", i++});
        data.push_back(new types::String(macro->getModule().c_str()));

        fields.insert({L"ParentIndex", i++});
        data.push_back(new types::Double(0));

        functionTable->resize(1, 1);
        functionTable->set(0, 0, functionTableStruct);
        return;
    }

    // append some information to an existing types::SingleStruct
    int i = 0;
    auto& fields = functionTable->get(0, 0)->getFields();
    auto& data = functionTable->get(0, 0)->getData();

    const auto& found = fileDescription.lower_bound(macro->getFileName());
    int previousSize = data[0]->getAs<types::String>()->getSize();

    types::String* functionName = data[i++]->getAs<types::String>();
    functionName->resize(previousSize + 1, 1);
    functionName->set(previousSize, 0, macro->getName().c_str());

    types::String* fileName = data[i++]->getAs<types::String>();
    fileName->resize(previousSize + 1, 1);
    fileName->set(previousSize, 0, macro->getFileName().c_str());

    types::Double* firstLine = data[i++]->getAs<types::Double>();
    firstLine->resize(previousSize + 1, 1);
    if (is_nested_function(fileDescription, found, macro))
    {
        firstLine->set(previousSize, 0, found->second.first);
    }
    else
    {
        firstLine->set(previousSize, 0, macro->getFirstLine());
    }

    types::String* libraryName = data[i++]->getAs<types::String>();
    libraryName->resize(previousSize + 1, 1);
    libraryName->set(previousSize, 0, macro->getModule().c_str());

    types::Double* parentIndex = data[i++]->getAs<types::Double>();
    parentIndex->resize(previousSize + 1, 1);
    if (is_nested_function(fileDescription, found, macro))
    {
        for (int i = 0; i < fileName->getSize() - 1; i++)
        {
            bool sameFileName = wcscmp(fileName->get(i), macro->getFileName().c_str()) == 0;
            bool samefirstLine = firstLine->get(i) == found->second.first;
            if (sameFileName && samefirstLine)
            {
                parentIndex->set(previousSize, 0, i + 1);
                break;
            }
        }
    }
    else
    {
        parentIndex->set(previousSize, 0, 0);
    }

}

void populateFunctionCoverage(types::Struct* functionCoverage, CoverModule* cm, types::Macro* macro)
{
    const CoverResult empty = CoverResult(L"", CoverMacroInfo(L"", L"", 0, 0, 0));

    // retrieve the function coverage information
    const auto& results = cm->getResults();
    const CoverResult* coverResultPtr = nullptr;
    const auto& map1 = results.find(macro->getModule());
    if (map1 != results.end())
    {
        const auto& map2 = map1->second.find(macro->getFileName());
        if (map2 != map1->second.end())
        {
            MacroLoc ml(macro->getName(), macro->getBody()->getLocation());
            const auto& found = map2->second.find(ml);
            if (found != map2->second.end())
            {
                coverResultPtr = &(found->second);
            }
        }
    }
    if (coverResultPtr == nullptr)
    {
        coverResultPtr = &empty;
    }
    const CoverResult& result = *coverResultPtr;

    // first call
    if (functionCoverage->getSize() == 0)
    {
        types::SingleStruct* functionCoverageStruct = new types::SingleStruct();

        int i = 0;
        auto& fields = functionCoverageStruct->getFields();
        auto& data = functionCoverageStruct->getData();

        fields.insert({L"NumCalls", i++});
        data.push_back(new types::UInt64(result.getCounter()));

        fields.insert({L"TotalTime", i++});
        data.push_back(new types::Double(std::pow(10, std::log10(result.getNanoTime()) - 9)));

        fields.insert({L"InstructionsCount", i++});
        types::UInt64* instrsCount = new types::UInt64(1, 2);
        instrsCount->set(0, 0, result.getInfo().instrsCount);
        instrsCount->set(0, 1, result.getUncInstrs());
        data.push_back(instrsCount);

        fields.insert({L"BranchesCount", i++});
        types::UInt64* branchesCount = new types::UInt64(1, 2);
        branchesCount->set(0, 0, result.getInfo().branchesCount);
        branchesCount->set(0, 1, result.getUncBranches());
        data.push_back(branchesCount);

        fields.insert({L"PathsCount", i++});
        data.push_back(new types::UInt64(result.getInfo().pathsCount));

        functionCoverage->resize(1, 1);
        functionCoverage->set(0, 0, functionCoverageStruct);
        return;
    }

    // append some information to an existing types::SingleStruct
    int i = 0;
    auto& fields = functionCoverage->get(0, 0)->getFields();
    auto& data = functionCoverage->get(0, 0)->getData();

    int previousSize = data[0]->getAs<types::String>()->getSize();

    types::UInt64* numCalls = data[i++]->getAs<types::UInt64>();
    numCalls->resize(previousSize + 1, 1);
    numCalls->set(previousSize, 0, result.getCounter());

    types::Double* totalTime = data[i++]->getAs<types::Double>();
    totalTime->resize(previousSize + 1, 1);
    totalTime->set(previousSize, 0, std::pow(10, std::log10(result.getNanoTime()) - 9));

    types::UInt64* instrsCount = data[i++]->getAs<types::UInt64>();
    instrsCount->resize(previousSize + 1, 2);
    instrsCount->set(previousSize, 0, result.getInfo().instrsCount);
    instrsCount->set(previousSize, 1, result.getUncInstrs());

    types::UInt64* branchesCount = data[i++]->getAs<types::UInt64>();
    branchesCount->resize(previousSize + 1, 2);
    branchesCount->set(previousSize, 0, result.getInfo().branchesCount);
    branchesCount->set(previousSize, 1, result.getUncBranches());

    types::UInt64* pathsCount = data[i++]->getAs<types::UInt64>();
    pathsCount->resize(previousSize + 1, 1);
    pathsCount->set(previousSize, 0, result.getInfo().pathsCount);
}

void populateLineCoverage(types::List* lineCoverage, std::map<std::wstring, std::pair<int, int>>& fileDescription, std::vector<Counter>::const_iterator& first, const std::vector<Counter>::const_iterator& last)
{
    types::Macro* macro = first->getMacro();

    // firstLine and lastLine are stored into the fileDescription set
    //
    // As the macros are in order of their definitions, nested-functions are
    // populated after their parent functions. The initial (eg. from parent
    // functions) firstLine and lastLine are re-used for their nesteds.
    int macroFirstLine = macro->getFirstLine();
    int macroLastLine = macro->getLastLine();
    const auto& found = fileDescription.lower_bound(macro->getFileName());
    if (is_nested_function(fileDescription, found, macro))
    {
        // nested functions are merged within their parents
        macroFirstLine = found->second.first;
        macroLastLine = found->second.second;
    }
    else if (is_sub_function(fileDescription, found, macro))
    {
        // sub functions are independant
        found->second.first = macroFirstLine;
        found->second.second = macroLastLine;
    }
    else
    {
        // this is a note parsed function, store for later use
        fileDescription.insert(found, {macro->getFileName(), std::make_pair(macroFirstLine, macroLastLine)});
    }

    // initialize with "not executable" flags
    types::Double* coverage = new types::Double(macroLastLine - macroFirstLine + 1, 2);
    for (int i = 0; i < coverage->getRows(); i++)
    {
        coverage->set(i, 0, -1.);
        coverage->set(i, 1, 0.);
    }

    // store counters
    for (std::vector<Counter>::const_iterator it = first; it < last; ++it)
    {
        // filter out function declaration timings, this is not clear for the end-user
        if (it->getExp()->isFunctionDec())
        {
            continue;
        }

        const Location& line = it->getExp()->getLocation();
        // multi-lines Exp (such as IfExp) only display its counter on its first
        // line (with the if keyword).
        //
        // Multiple Exp might be present on the same line, in that case the usual
        // way to report is to have the counter of the first Exp and the
        // cumulated time displayed.
        int l = line.first_line;
        if (coverage->get(l - macroFirstLine, 0) <= 0.)
        {
            coverage->set(l - macroFirstLine, 0, double(it->get()));
        }
        double seconds = std::pow(10, std::log10(it->getNanoTime()) - 9);
        coverage->set(l - macroFirstLine, 1, coverage->get(l - macroFirstLine, 1) + seconds);
    }

    lineCoverage->set(lineCoverage->getSize(), coverage);
}

void populate(CoverModule* cm, types::Struct* functionTable, types::Struct* functionCoverage, types::List* lineCoverage)
{
    const std::vector<Counter>& counters = cm->getCounters();

    // store firstLine, lastLine information per filename
    std::map<std::wstring, std::pair<int, int>> fileDescription;

    // at least, one macro is instrumented
    std::vector<Counter>::const_iterator first = counters.begin();
    std::vector<Counter>::const_iterator last = cm->upper_bound(first, counters.end(), first->getMacro());

    populateFunctionTable(functionTable, fileDescription, first->getMacro());
    populateFunctionCoverage(functionCoverage, cm, first->getMacro());
    populateLineCoverage(lineCoverage, fileDescription, first, last);

    while (last != counters.end())
    {
        first = last;
        last = cm->upper_bound(first, counters.end(), first->getMacro());

        populateFunctionTable(functionTable, fileDescription, first->getMacro());
        populateFunctionCoverage(functionCoverage, cm, first->getMacro());
        populateLineCoverage(lineCoverage, fileDescription, first, last);
    }
}

} /* namespace */
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_profileGetInfo(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "profileGetInfo", 0);
        return types::Function::ReturnValue::Error;
    }
    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d expected.\n"), "profileGetInfo", 1);
        return types::Function::ReturnValue::Error;
    }

    CoverModule* cm = CoverModule::getInstance();
    if (cm == nullptr)
    {
        Scierror(999, _("%s: profile is disabled.\n"), "profileGetInfo");
        return types::Function::ReturnValue::Error;
    }
    cm->collect();

    types::Struct* functionTable = new types::Struct();
    types::Struct* functionCoverage = new types::Struct();
    types::List* lineCoverage = new types::List();

    if (!cm->getCounters().empty())
    {
        populate(cm, functionTable, functionCoverage, lineCoverage);
    }

    int i = 0;
    types::String* header = new types::String(4, 1);
    header->set(i++, "ProfilerStatistics");
    header->set(i++, "FunctionTable");
    header->set(i++, "FunctionCoverage");
    header->set(i++, "LineCoverage");

    types::TList* tlist = new types::TList();
    tlist->append(header);
    tlist->append(functionTable);
    tlist->append(functionCoverage);
    tlist->append(lineCoverage);
    out.push_back(tlist);

    return types::Function::OK;
}
