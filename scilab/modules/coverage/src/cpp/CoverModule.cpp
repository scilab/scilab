/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#ifdef _MSC_VER
#pragma comment(lib, "../../bin/libxml2.lib")
#endif

#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

#include "CoverModule.hxx"
#include "CovHTMLCodePrinter.hxx"
#include "cover_tools.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alltypes.hxx"
#include "coverage_instance.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "expandPathVariable.h"
#include "createdirectory.h"
#include "copyfile.h"
#include "isdir.h"
#include "findfiles.h"
#include "freeArrayOfString.h"
}

#ifdef _MSC_VER
#define DEFAULT_FILESPEC L"*.*"
#else
#define DEFAULT_FILESPEC L"*"
#endif

namespace coverage
{

CoverModule * CoverModule::instance = nullptr;

CoverModule::CoverModule(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods) : visitor(*this)
{
    getMacros(paths_mods);
    getBuiltins(paths_mods);
    ast::CoverageInstance::setCoverage(this);
}

CoverModule::CoverModule(const std::vector<std::wstring> & moduleNames) : CoverModule(getModule(moduleNames))
{
    ast::CoverageInstance::setCoverage(this);
}

CoverModule::CoverModule() : visitor(*this)
{
    ast::CoverageInstance::setCoverage(this);
}

CoverModule::~CoverModule()
{
    for (auto & counter : counters)
    {
        counter.getExp()->setCoverId(0);
    }
    for (auto & p : callCounters)
    {
        p.first->DecreaseRef();
        //p.first->killMe();
    }

    ast::CoverageInstance::setCoverage(nullptr);
}

const std::vector<std::pair<std::wstring, std::wstring>> CoverModule::getModule(const std::vector<std::wstring> & moduleNames)
{
    const std::wstring _path = std::wstring(L"SCI") + DIR_SEPARATORW + L"modules" + DIR_SEPARATORW;
    wchar_t * __path = expandPathVariableW((wchar_t *)_path.c_str());
    const std::wstring path(__path);
    FREE(__path);

    if (moduleNames.size() == 1 && moduleNames.back() == L"all")
    {
        int size = -1;
        wchar_t ** files = findfilesW(path.c_str(), DEFAULT_FILESPEC, &size, FALSE);
        if (size > 0 && files)
        {
            std::vector<std::pair<std::wstring, std::wstring>> paths;
            for (int i = 0; i < size; ++i)
            {
                const std::wstring modulePath = path + files[i];
                if (isdirW(modulePath.c_str()))
                {
                    paths.emplace_back(modulePath, files[i]);
                }
            }
            freeArrayOfWideString(files, size);
            return paths;
        }

        return{};
    }
    else
    {
        std::vector<std::pair<std::wstring, std::wstring>> paths;
        for (const auto & name : moduleNames)
        {
            paths.emplace_back(path + name, name);
        }
        return paths;
    }
}

void CoverModule::getMacros(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods)
{
    for (const auto & p : paths_mods)
    {
        std::wstring _path = p.first + DIR_SEPARATORW + L"macros";
        getMacrosFromDir(_path, p.second);
    }
}

void CoverModule::getMacrosFromDir(const std::wstring & path, const std::wstring & module)
{
    std::wstring _path = path + DIR_SEPARATORW + L"lib";
    getMacros(_path, module);

    int size = -1;
    _path = path + DIR_SEPARATORW;

    wchar_t ** files = findfilesW(_path.c_str(), DEFAULT_FILESPEC, &size, FALSE);
    if (size > 0 && files)
    {
        for (int i = 0; i < size; ++i)
        {
            std::wstring _file = _path + files[i];
            if (isdirW(_file.c_str()))
            {
                getMacrosFromDir(_file, module);
            }
        }
        freeArrayOfWideString(files, size);
    }
}

void CoverModule::getMacros(const std::wstring & path, const std::wstring & module)
{
    std::unordered_set<std::wstring> _macros;
    wchar_t * pwstPathLib = expandPathVariableW((wchar_t *)path.c_str());
    std::wstring libPath(pwstPathLib);
    FREE(pwstPathLib);

    char * libFile = wide_string_to_UTF8(libPath.c_str());

    if (getStringFromXPath(libFile, "//scilablib/macro/@name", _macros))
    {
        for (const auto & name : _macros)
        {
            types::InternalType * pIT = symbol::Context::getInstance()->get(symbol::Symbol(name));
            if (pIT && pIT->isMacroFile())
            {
                types::MacroFile * pMF = static_cast<types::MacroFile *>(pIT);
                if (types::Macro * macro = pMF->getMacro())
                {
                    const std::wstring & file = pMF->getPath();
                    std::size_t pos = file.find_last_of(L'.');
                    if (pos != std::string::npos)
                    {
                        instrumentMacro(module, file.substr(0, pos) + L".sci", macro);
                    }
                    else
                    {
                        instrumentMacro(module, file, macro);
                    }
                }
            }
        }
    }
    FREE(libFile);
}

void CoverModule::getBuiltins(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods)
{
    for (const auto & p : paths_mods)
    {
        std::list<types::Callable *> lst;
        if (symbol::Context::getInstance()->getFunctionList(lst, p.second))
        {
            for (auto pCall : lst)
            {
                if (pCall->isFunction())
                {
                    pCall->IncreaseRef();
                    functions.emplace(p.second, static_cast<types::Function *>(pCall));
                    callCounters.emplace(pCall, CallCounter());
                }
            }
        }
    }
}

void CoverModule::instrumentMacro(const std::wstring & module, const std::wstring & path, types::Macro * macro)
{
    const std::map<symbol::Variable *, types::Macro *> & submacros = macro->getSubMacros();
    for (const auto & p : submacros)
    {
        instrumentSingleMacro(module, path, p.second, true);
    }

    instrumentSingleMacro(module, path, macro, true);
}

void CoverModule::instrumentSingleMacro(const std::wstring & module, const std::wstring & path, types::Macro * macro, bool instrumentInners)
{
    macro->IncreaseRef();
    visitor.setMacro(macro);
    macro->getBody()->accept(visitor);
    macros.emplace(macro, CoverMacroInfo(module, path, visitor.getInstrsCount(), visitor.getBranchesCount(), visitor.getPathsCount()));
    callCounters.emplace(static_cast<types::Callable *>(macro), CallCounter());
    functions.emplace(module, static_cast<types::Callable *>(macro));

    if (instrumentInners)
    {
        // We make a copy since the call to instrumentSingleMacro will modify visitor.getInnerMacros()
        const std::vector<types::Macro *> inners = visitor.getInnerMacros();
        for (auto inner : inners)
        {
            instrumentSingleMacro(module, path, inner, true);
        }
    }
}

void CoverModule::add(types::Macro * macro, ast::Exp * e)
{
    if (e)
    {
        counters.emplace_back(macro, e);
        const std::size_t id = counters.size() + 1;
        e->setCoverId(id);
    }
}

void CoverModule::invoke(types::Callable * f)
{
    if (f->isMacroFile())
    {
        f = static_cast<types::MacroFile *>(f)->getMacro();
    }
    auto i = callCounters.find(f);
    if (i != callCounters.end())
    {
        i->second.inc();
    }
}

void CoverModule::invoke(const uint64_t id)
{
    counters[id - 2].inc();
}

bool CoverModule::getStringFromXPath(char * filePath, const char * xpquery, std::unordered_set<std::wstring> & set)
{
    xmlDocPtr doc = xmlReadFile(filePath, "utf-8", XML_PARSE_NOWARNING);
    if (!doc)
    {
        return false;
    }

    if (!doc->encoding || stricmp((const char *)doc->encoding, "utf-8") != 0)
    {
        xmlFreeDoc(doc);
        return false;
    }

    xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
    xmlXPathObjectPtr xp = xmlXPathEval((const xmlChar *)xpquery, ctxt);
    xmlNodeSetPtr nodeSet = xp->nodesetval;

    if (nodeSet)
    {
        for (unsigned int i = 0; i < nodeSet->nodeNr; ++i)
        {
            const char * content = (const char *)xmlNodeGetContent(nodeSet->nodeTab[i]);
            wchar_t * ws = to_wide_string(content);
            xmlFree(const_cast<char *>(content));
            set.emplace(ws);
            FREE(ws);
        }
    }

    xmlXPathFreeObject(xp);
    xmlXPathFreeContext(ctxt);
    xmlFreeDoc(doc);

    return nodeSet;
}

void CoverModule::collect()
{
    if (!counters.empty())
    {
        std::vector<Counter>::const_iterator b = counters.begin();
        types::Macro * current = counters.front().getMacro();
        for (std::vector<Counter>::const_iterator i = b, e = counters.end(); i != e; ++i)
        {
            if (i->getMacro() != current || std::next(i) == e)
            {
                const std::wstring & name = current->getName();
                CoverMacroInfo & info = macros.find(current)->second;
                const uint64_t counter = callCounters[current].get();
                auto & map1 = results[info.macroModule];
                auto & map2 = map1[info.macroFilePath];
                MacroLoc ml(name, current->getBody()->getLocation());
                auto j = map2.find(ml);
                if (j == map2.end())
                {
                    CoverResult & result = map2.emplace(ml, CoverResult(name, info)).first->second;
                    result.setCounter(counter);

                    if (std::next(i) == e)
                    {
                        result.populate(b, e);
                    }
                    else
                    {
                        result.populate(b, i);
                        b = i;
                        current = i->getMacro();
                    }
                }
                allCounters[info.macroModule][name] = std::pair<bool, uint64_t>(true, counter);
            }
        }

        for (const auto & p : functions)
        {
            if (p.second->isFunction())
            {
                allCounters[p.first][p.second->getName()] = std::pair<bool, uint64_t>(false, callCounters[p.second].get());
            }
        }
    }
}

void CoverModule::print()
{
    /*std::wcerr << L"Builtin calls" << std::endl << tools::getUnderline(L"Builtin calls") << std::endl;
      for (const auto & p : callCounters)
      {
      if (p.second.get())
      {
      std::wcerr << p.first->getName() << L" called " << p.second.get() << L" time" << tools::getPlural(p.second.get()) << L"." << std::endl;
      }
      }

      std::wcerr << std::endl;*/

    /*      for (const auto & counter : counters)
            {
            if (counter.get())
            {

            const std::wstring & name = counter.getMacro()->getName();
            std::wcerr << L"Macro " << name << L" visited at " << counter.getExp()->getLocation() << std::endl;


            //<< L"Seq not visited:" << std::endl;

            }
            }
            */
    //collect();
}

void CoverModule::toHTML(const std::wstring & outputDir)
{
    bool nomodules = false;
    wchar_t * _outputDir = expandPathVariableW((wchar_t *)outputDir.c_str());
    createdirectoryW(_outputDir);

    if (results.size() == 1 && results.begin()->first == L"" && results.begin()->second.size() == 1 && results.begin()->second.begin()->first == L"")
    {
        for (const auto & p : macros)
        {
            writeMacroHTMLReport(p.first, results.begin()->second.begin()->second, _outputDir);
        }
        nomodules = true;
    }

    if (!nomodules)
    {
        std::map<std::wstring, std::pair<uint64_t, double>> modulesStats;

        // We make all the reports for the macros
        for (auto & p1 : results)
        {
            const std::wstring & moduleName = p1.first;
            const std::wstring __outputDir = std::wstring(_outputDir) + DIR_SEPARATORW + moduleName;
            createdirectoryW((wchar_t *)__outputDir.c_str());
            uint64_t totalCalls = 0;
            uint64_t totalInstrs = 0;
            uint64_t uncoveredInstrs = 0;

            for (auto & p2 : p1.second)
            {
                writeMacroHTMLReport(p2.first, moduleName, p2.second, __outputDir);
            }

            // Now we make the module's reports
            std::wostringstream out;

            out << L"<html lang=\'en\'>\n"
                << L"<meta charset=\'UTF-8\'>\n"
                << L"<head>\n"
                << L"<link rel=\'icon\' href=\'../favicon.ico\'/>\n"
                << L"<title>Scilab | Module " << moduleName << L" | Scilab\'s code coverage" << L"</title>\n"
                << L"<style type=\'text/css\' media=\'all\'>\n"
                << L"@import url(\'../mod_style.css\');\n"
                << L"</style>\n"
                << L"<script type=\'text/javascript\' src=\'../module.js\'></script>"
                << L"</head>\n"
                << L"<body>\n";

            bool altern = false;
            unsigned int tableid = 0;
            unsigned int buttonid = 0;

            out << L"<h2 class=\'title\'>Macros calls</h2>\n"
                << L"<table class='module'>\n"
                << L"<tr><td><div class=\'macros_stats\'>\n"
                << L"<div class=\'macros_cell\'><table id=\'table" << tableid << L"\'>\n"
                << L"<tr class=\'col_name\'><td>Name&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid, 0, false) << L"</td><td>File</td><td>Calls&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 1, 2, true) << L"</td><td>Covered&nbsp" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 2, 3, false) << L"</td></tr>\n";

            for (const auto & t : getOrderedResults(moduleName))
            {
                const CoverResult & res = *std::get<0>(t);
                const std::wstring & macroFilename = *std::get<1>(t);
                const std::wstring & macroName = *std::get<2>(t);

                totalInstrs += res.getInfo().instrsCount;
                uncoveredInstrs += res.getUncInstrs();

                std::size_t pos = macroFilename.find_last_of(L'.');
                std::wstring filename = macroFilename.substr(0, pos);
                pos = filename.find_last_of(L"\\/");
                if (pos != std::string::npos)
                {
                    filename = filename.substr(pos + 1);
                }
                std::wstring filepath = encodeFilename(filename);

                const std::wstring countercls = res.counter == 0 ? L"null_stats" : L"stats";
                const std::wstring trcls = altern ? L"altern1" : L"altern2";
                out << L"<tr class=\'" << trcls << L"\'><td><span class=\'cmp\'>" << macroName << L"</span></td>"
                    << L"<td><a class=\'filepath' href=\'" << filepath << L".html\'>" << moduleName << L"/macros/" << filename << L".sci</a></td>"
                    << L"<td class=\'" << countercls << L"\'><span class=\'cmp\'>" << res.counter << L"</span></td>"
                    << L"<td><span class=\'inline_percent\'><span class=\'cmp\'>" << res.getCovInstrsPercent() << L"</span> %</span>";
                CovHTMLCodePrinter::getDivPercent(out, res.getCovInstrsPercent());
                out << L"</td></tr>\n";

                altern = !altern;
            }

            ++tableid;
            buttonid += 3;

            out << L"</table></div></div></td></tr>\n"
                << L"</table>\n";

            // Now we print the number of calls for builtins.

            altern = false;

            out << L"<h2 class=\'title\'>Builtins calls</h2>\n"
                << L"<table class='module'>\n"
                << L"<tr><td><div class='modulePath'>" << moduleName << L"</div></td></tr>\n"
                << L"<tr><td><div class=\'builtins_stats\'>\n"
                << L"<div class=\'builtins_cell\'><table id=\'table" << tableid << L"\'>\n"
                << L"<tr class=\'col_name\'><td>Name&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid, 0, false) << L"</td><td class=\'col_name\'>Calls&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 1, 1, true) << L"</td></tr>\n";

            for (const auto & p : getBuiltinStats(moduleName))
            {
                const std::wstring countercls = p.second == 0 ? L"null_stats" : L"stats";
                const std::wstring trcls = altern ? L"altern1" : L"altern2";
                out << L"<tr class=\'" << trcls << L"\'><td><span class=\'cmp\'>" << *p.first << L"</span></td><td class=\'" << countercls << L"\'><span class=\'cmp\'>" << p.second << L"</span></td></tr>\n";
                altern = !altern;
                totalCalls += p.second;
            }

            out << L"</table></div>\n"
                << L"</div></td></tr>\n"
                << L"</table>\n"
                << L"</body>\n"
                << L"</html>\n";

            out.flush();
            writeFile(out, __outputDir, moduleName + L".html");

            const double percent = totalInstrs ? std::round(100. * (1. - (double)uncoveredInstrs / (double)totalInstrs)) : 100.;
            modulesStats.emplace(moduleName, std::pair<uint64_t, double>(totalCalls, percent));
        }

        // Now we must output the results for all the modules
        const std::wstring __outputDir(_outputDir);
        std::wostringstream out;

        out << L"<html lang=\'en\'>\n"
            << L"<meta charset=\'UTF-8\'>\n"
            << L"<head>\n"
            << L"<link rel=\'icon\' href=\'favicon.ico\'/>\n"
            << L"<title>Scilab\'s code coverage" << L"</title>\n"
            << L"<style type=\'text/css\' media=\'all\'>\n"
            << L"@import url(\'mod_style.css\');\n"
            << L"</style>\n"
            << L"<script type=\'text/javascript\' src=\'module.js\'></script>"
            << L"</head>\n"
            << L"<body>\n";

        bool altern = false;
        unsigned int tableid = 0;
        unsigned int buttonid = 0;

        out << L"<h2 class=\'title\'>Modules results</h2>\n"
            << L"<table class='module'>\n"
            << L"<tr><td><div class=\'macros_stats\'>\n"
            << L"<div class=\'macros_cell\'><table id=\'table" << tableid << L"\'>\n"
            << L"<tr class=\'col_name\'><td>Name&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid, 0, false) << L"</td><td>Builtin calls&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 1, 1, true) << L"</td><td>Covered&nbsp" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 2, 2, false) << L"</td></tr>\n";

        for (const auto & p : modulesStats)
        {
            const std::wstring & moduleName = p.first;
            const uint64_t builtinCalls = p.second.first;
            const double percent = p.second.second;
            const std::wstring countercls = builtinCalls ? L"stats" : L"null_stats";
            const std::wstring trcls = altern ? L"altern1" : L"altern2";
            out << L"<tr class=\'" << trcls << L"\'>"
                << L"<td><span class=\'cmp\'><a class=\'filepath' href=\'" << moduleName << L"/" << moduleName << L".html\'>" << moduleName << L"</a></span></td>"
                << L"<td class=\'" << countercls << L"\'><span class=\'cmp\'>" << builtinCalls << L"</span></td>"
                << L"<td><span class=\'inline_percent\'><span class=\'cmp\'>" << percent << L"</span> %</span>";

            CovHTMLCodePrinter::getDivPercent(out, percent);
            out << L"</tr>\n";
            altern = !altern;
        }

        out << L"</table></div>\n"
            << L"</div></td></tr>\n"
            << L"</table>\n"
            << L"</body>\n"
            << L"</html>\n";

        out.flush();
        writeFile(out, __outputDir, L"report.html");
    }

    FREE(_outputDir);
    copyDataFiles(outputDir);
}

void CoverModule::copyDataFiles(const std::wstring & outputDir)
{
    const std::wstring _outputDir = outputDir + DIR_SEPARATORW;
    const std::wstring _inputDir = std::wstring(L"SCI") + DIR_SEPARATORW + L"modules" + DIR_SEPARATORW + L"coverage" + DIR_SEPARATORW + L"data";
    copyFile(_inputDir, _outputDir, L"scilab_code.css");
    copyFile(_inputDir, _outputDir, L"src_style.css");
    copyFile(_inputDir, _outputDir, L"mod_style.css");
    copyFile(_inputDir, _outputDir, L"favicon.ico");
    copyFile(_inputDir, _outputDir, L"module.js");
}

void CoverModule::copyFile(const std::wstring & inDir, const std::wstring & outDir, const std::wstring & filename)
{
    const std::wstring in = inDir + DIR_SEPARATORW + filename;
    const std::wstring out = outDir + DIR_SEPARATORW + filename;
    wchar_t * _input = expandPathVariableW((wchar_t *)in.c_str());
    wchar_t * _output = expandPathVariableW((wchar_t *)out.c_str());
    CopyFileFunction(_output, _input);
    FREE(_input);
    FREE(_output);
}

void CoverModule::writeFile(const std::wostringstream & out, const std::wstring & outputDir, const std::wstring & filename)
{
    const std::string code = scilab::UTF8::toUTF8(out.str().c_str());
    const std::string _filename = scilab::UTF8::toUTF8(outputDir + DIR_SEPARATORW + filename);
    std::fstream file(_filename, std::ios::out | std::ios::trunc);
    file.write(code.c_str(), code.size());
    file.close();
}

std::set<CoverModule::__Res1, CoverModule::__Compare1> CoverModule::getOrderedResults(const std::wstring & moduleName) const
{
    std::set<__Res1, __Compare1> set;
    auto i = results.find(moduleName);
    if (i != results.end())
    {
        for (const auto & p : i->second)
        {
            const std::wstring & macroFilename = p.first;
            for (const auto & pp : p.second)
            {
                const std::wstring & macroName = pp.first.name;
                const Location & macroLoc = pp.first.loc;
                const CoverResult & res = pp.second;
                set.emplace(__Res1(&res, &macroFilename, &macroName, &macroLoc));
            }
        }
    }

    return set;
}

std::set<CoverModule::__Res2, CoverModule::__Compare2> CoverModule::getBuiltinStats(const std::wstring & moduleName) const
{
    std::set<__Res2, __Compare2> set;
    auto i = allCounters.find(moduleName);
    if (i != allCounters.end())
    {
        for (const auto & p : i->second)
        {
            if (!p.second.first)
            {
                set.emplace(__Res2(&p.first, p.second.second));
            }
        }
    }

    return set;
}

std::vector<std::pair<types::Callable *, uint64_t>> CoverModule::getFunctionCalls(const std::wstring & moduleName, const bool builtin) const
{
    struct _Res
    {
        types::Callable * const fptr;
        const uint64_t counter;

        _Res(types::Callable * const _fptr, const uint64_t _counter) : fptr(_fptr), counter(_counter) { }
        inline bool operator<(const _Res & res) const
        {
            return (counter < res.counter) || (counter == res.counter && fptr->getName() < res.fptr->getName());
        }
    };
    std::set<_Res> set;
    std::vector<std::pair<types::Callable *, uint64_t>> calls;
    auto range = functions.equal_range(moduleName);
    for (auto fptr = range.first; fptr != range.second; ++fptr)
    {
        if ((builtin && fptr->second->isFunction()) || (!builtin && fptr->second->isMacro()))
        {
            auto i = callCounters.find(static_cast<types::Callable *>(fptr->second));
            if (i != callCounters.end())
            {
                set.emplace(fptr->second, i->second.get());
            }
        }
    }

    calls.reserve(set.size());
    for (const auto r : set)
    {
        calls.emplace_back(r.fptr, r.counter);
    }

    return calls;
}

ast::Exp * CoverModule::getTree(const std::wstring & path)
{
    if (!path.empty())
    {
        std::ifstream src(scilab::UTF8::toUTF8(path), std::ios::in | std::ios::binary | std::ios::ate);
        if (src.is_open())
        {
            src.seekg(0, src.end);
            int len = src.tellg();
            src.seekg(0, src.beg);
            char * buffer = new char[len + 1];
            buffer[len] = '\0';
            src.read(buffer, len);
            src.close();

            wchar_t * _wstr = to_wide_string(buffer);
            delete[] buffer;
            Parser parser;
            parser.parse(_wstr);
            FREE(_wstr);

            return parser.getTree();
        }
    }
    return nullptr;
}

const std::wstring CoverModule::getName(const std::wstring & path)
{
    std::size_t pos = path.find_last_of(L'.');
    std::wstring name = path.substr(0, pos);
    pos = name.find_last_of(L"\\/");
    if (pos != std::string::npos)
    {
        name = name.substr(pos + 1);
    }
    return name;
}

void CoverModule::writeMacroHTMLReport(types::Macro * macro, std::map<MacroLoc, CoverResult> & results, const std::wstring & outputDir)
{
    std::list<symbol::Variable *> * in = macro->getInputs();
    std::list<symbol::Variable *> * out = macro->getOutputs();
    ast::SeqExp & body = *macro->getBody()->clone();

    ast::exps_t & _in = *new ast::exps_t();
    ast::exps_t & _out = *new ast::exps_t();
    for (const auto i : *in)
    {
        _in.emplace_back(new ast::SimpleVar(Location(), i->getSymbol()));
    }
    for (const auto o : *out)
    {
        _out.emplace_back(new ast::SimpleVar(Location(), o->getSymbol()));
    }
    ast::FunctionDec * fdec = new ast::FunctionDec(Location(), symbol::Symbol(macro->getName()), *new ast::ArrayListVar(Location(), _in), *new ast::ArrayListVar(Location(), _out), body);

    writeMacroHTMLReport(fdec, macro->getName() + L".html", L"", L"", results, outputDir);
}

void CoverModule::writeMacroHTMLReport(ast::Exp * tree, const std::wstring & filename, const std::wstring & path, const std::wstring & moduleName, std::map<MacroLoc, CoverResult> & results, const std::wstring & outputDir)
{
    std::wostringstream out;
    std::wstring mod, prev;
    if (!moduleName.empty())
    {
        mod = L" | Module " + moduleName;
        prev = L"../";
    }
    std::wstring pa;
    if (!path.empty())
    {
        pa = L" | " + path;
    }

    out << L"<html lang=\'en\'>\n"
        << L"<meta charset=\'UTF-8\'>\n"
        << L"<head>\n"
        << L"<link rel=\'icon\' href=\'../favicon.ico\'/>\n"
        << L"<title>Scilab" << mod << pa << L" | Scilab\'s code coverage" << L"</title>\n"
        << L"<style type=\'text/css\' media=\'all\'>\n"
        << L"@import url(\'" << prev << "scilab_code.css\');\n"
        << L"@import url(\'" << prev << "src_style.css\');\n"
        << L"</style>\n"
        << L"<script>\n"
        << L"function show(did,fid) {\n"
        << L"  x = document.getElementById(did).style;\n"
        << L"  y = document.getElementById(fid);\n"
        << L"  x.visibility = 'visible';\n"
        << L"  x.display = 'block';\n"
        << L"  x.height = 'auto';\n"
        << L"  x.left = y.offsetLeft + 'px';\n"
        << L"  x.top = y.offsetTop + y.offsetHeight + 'px';\n"
        << L"}\n"
        << L"function hide(did) {\n"
        << L"  document.getElementById(did).style.visibility = \'hidden\';\n"
        << L"}\n"
        << L"</script>\n"
        << L"</head>\n"
        << L"<body>\n"
        << L"<h2 class=\'title\'>Coverage and Profiling report</h2>\n"
        << L"<table class='sourceFile'>\n";

    if (!path.empty())
    {
        out << L"<tr><td><div class='sourcePath'>" << path << L"</div></td></tr>\n";
    }
    if (!results.empty())
    {
        for (const auto & p : results)
        {
            out << L"<tr><td><div class=\'allmacstats\'>\n";
            CovHTMLCodePrinter::getFunctionStats(out, p.first, p.second);
            out << L"</div></td></tr\n";
        }
    }

    out << L"<tr><td><div class=\'source\'>\n"
        << L"<table>\n";

    CovHTMLCodePrinter printer(out, results);
    CodePrinterVisitor visitor(printer);
    tree->accept(visitor);
    printer.close();
    delete tree;

    out << L"</table>\n"
        << L"</div></td></tr>\n"
        << L"</table>\n"
        << L"</body>\n"
        << L"</html>\n";

    out.flush();
    writeFile(out, outputDir, filename);
}

bool CoverModule::writeMacroHTMLReport(const std::wstring & path, const std::wstring & moduleName, std::map<MacroLoc, CoverResult> & results, const std::wstring & outputDir)
{
    if (ast::Exp * tree = getTree(path))
    {
        writeMacroHTMLReport(tree, getName(path) + L".html", path, moduleName, results, outputDir);
        return true;
    }

    return false;
}

std::wstring CoverModule::encodeFilename(const std::wstring & name)
{
    std::wostringstream wos;
    for (const auto c : name)
    {
        URLEncoder::replace(wos, c);
    }
    return wos.str();
}

void CoverModule::merge(const std::vector<std::wstring> & paths, const std::wstring & out)
{
    CoverModule cm;
    for (const auto path : paths)
    {
        cm.load(path);
    }
    cm.save(out);
}

void CoverModule::toHTML(const std::wstring & inBin, const std::wstring & outDir)
{
    CoverModule cm;
    cm.load(inBin);
    cm.toHTML(outDir);
}

void CoverModule::save(const std::wstring & path) const
{
    if (!path.empty())
    {
        std::fstream out(scilab::UTF8::toUTF8(path), std::ios::out | std::ios::binary);
        if (out.is_open())
        {
            toBin(out);
            out.close();
        }
    }
}

void CoverModule::load(const std::wstring & path)
{
    if (!path.empty())
    {
        std::fstream in(scilab::UTF8::toUTF8(path), std::ios::in | std::ios::binary);
        if (in.is_open())
        {
            fromBin(*this, in);
            in.close();
        }
    }
}

void CoverModule::toBin(std::fstream & out) const
{
    // Save results
    CoverModule::write(out, (uint64_t)results.size());
    for (const auto & p : results)
    {
        CoverModule::write(out, p.first);
        CoverModule::write(out, (uint64_t)p.second.size());
        for (const auto & pp : p.second)
        {
            CoverModule::write(out, pp.first);
            CoverModule::write(out, (uint64_t)pp.second.size());
            for (const auto & ppp : pp.second)
            {
                CoverModule::write(out, ppp.first.name);
                CoverModule::write(out, ppp.first.loc);
                ppp.second.toBin(out);
            }
        }
    }

    // Save allcounters
    CoverModule::write(out, (uint64_t)allCounters.size());
    for (const auto & p : allCounters)
    {
        CoverModule::write(out, p.first);
        CoverModule::write(out, (uint64_t)p.second.size());
        for (const auto & pp : p.second)
        {
            CoverModule::write(out, pp.first);
            CoverModule::write(out, pp.second.first);
            CoverModule::write(out, pp.second.second);
        }
    }
}

void CoverModule::fromBin(std::fstream & in)
{
    fromBin(*this, in);
}

void CoverModule::fromBin(CoverModule & cm, std::fstream & in)
{
    // Load results
    uint64_t size = CoverModule::readUint64_t(in);
    for (uint64_t i = 0; i < size; ++i)
    {
        const std::wstring moduleName = CoverModule::readWstring(in);
        const uint64_t sizee = CoverModule::readUint64_t(in);
        auto it = cm.results.find(moduleName);
        if (it == cm.results.end())
        {
            it = cm.results.emplace(moduleName, std::unordered_map<std::wstring, std::map<MacroLoc, CoverResult>>()).first;
        }
        auto & map = it->second;
        for (uint64_t j = 0; j < sizee; ++j)
        {
            const std::wstring macroFilename = CoverModule::readWstring(in);
            const uint64_t sizeee = CoverModule::readUint64_t(in);
            auto it = map.find(macroFilename);
            if (it == map.end())
            {
                it = map.emplace(macroFilename, std::map<MacroLoc, CoverResult>()).first;
            }
            auto & mapp = it->second;
            for (uint64_t k = 0; k < sizeee; ++k)
            {
                const std::wstring macroName = CoverModule::readWstring(in);
                const Location macroLoc = CoverModule::readLocation(in);
                const MacroLoc ml(macroName, macroLoc);
                auto it = mapp.find(ml);
                if (it == mapp.end())
                {
                    mapp.emplace(ml, CoverResult::fromBin(in));
                }
                else
                {
                    it->second.merge(CoverResult::fromBin(in));
                }
            }
        }
    }

    // Load allcounters
    size = CoverModule::readUint64_t(in);
    for (uint64_t i = 0; i < size; ++i)
    {
        const std::wstring moduleName = CoverModule::readWstring(in);
        const uint64_t sizee = CoverModule::readUint64_t(in);
        auto it = cm.allCounters.find(moduleName);
        if (it == cm.allCounters.end())
        {
            it = cm.allCounters.emplace(moduleName, std::unordered_map<std::wstring, std::pair<bool, uint64_t>>()).first;
        }
        auto & map = it->second;
        for (uint64_t j = 0; j < sizee; ++j)
        {
            const std::wstring funName = CoverModule::readWstring(in);
            const bool ismacro = CoverModule::readBool(in);
            const uint64_t counter = CoverModule::readUint64_t(in);
            auto it = map.find(funName);
            if (it == map.end())
            {
                map.emplace(funName, std::pair<bool, uint64_t>(ismacro, counter));
            }
            else
            {
                auto & p = it->second;
                if (p.first == ismacro)
                {
                    p.second += counter;
                }
            }
        }
    }
}

}
