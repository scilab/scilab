/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#define DEFAULT_FILESPEC "*.*"
#else
#define DEFAULT_FILESPEC "*"
#endif

namespace coverage
{

CoverModule * CoverModule::instance = nullptr;

CoverModule::CoverModule(const std::vector<std::pair<std::string, std::string>> & paths_mods) : visitor(*this)
{
    getMacros(paths_mods);
    getBuiltins(paths_mods);
    ast::CoverageInstance::setCoverage(this);
}

CoverModule::CoverModule(const std::vector<std::string> & moduleNames) : CoverModule(getModule(moduleNames))
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

const std::vector<std::pair<std::string, std::string>> CoverModule::getModule(const std::vector<std::string> & moduleNames)
{
    const std::string _path = std::string("SCI") + DIR_SEPARATOR + "modules" + DIR_SEPARATOR;
    char * __path = expandPathVariable(_path.c_str());
    const std::string path(__path);
    FREE(__path);

    if (moduleNames.size() == 1 && moduleNames.back() == "all")
    {
        int size = -1;
        char** files = findfiles(path.c_str(), DEFAULT_FILESPEC, &size, FALSE);
        if (size > 0 && files)
        {
            std::vector<std::pair<std::string, std::string>> paths;
            for (int i = 0; i < size; ++i)
            {
                const std::string modulePath = path + files[i];
                if (isdir(modulePath.c_str()))
                {
                    paths.emplace_back(modulePath, files[i]);
                }
            }
            freeArrayOfString(files, size);
            return paths;
        }

        return{};
    }
    else
    {
        std::vector<std::pair<std::string, std::string>> paths;
        for (const auto & name : moduleNames)
        {
            paths.emplace_back(path + name, name);
        }
        return paths;
    }
}

void CoverModule::getMacros(const std::vector<std::pair<std::string, std::string>> & paths_mods)
{
    for (const auto & p : paths_mods)
    {
        std::string _path = p.first + DIR_SEPARATOR + "macros";
        getMacrosFromDir(_path, p.second);
    }
}

void CoverModule::getMacrosFromDir(const std::string & path, const std::string & module)
{
    std::string _path = path + DIR_SEPARATOR + "lib";
    getMacros(_path, module);

    int size = -1;
    _path = path + DIR_SEPARATOR;

    char ** files = findfiles(_path.c_str(), DEFAULT_FILESPEC, &size, FALSE);
    if (size > 0 && files)
    {
        for (int i = 0; i < size; ++i)
        {
            std::string _file = _path + files[i];
            if (isdir(_file.c_str()))
            {
                getMacrosFromDir(_file, module);
            }
        }
        freeArrayOfString(files, size);
    }
}

void CoverModule::getMacros(const std::string & path, const std::string & module)
{
    std::unordered_set<std::string> _macros;
    char * pstPathLib = expandPathVariable(path.c_str());
    std::string libPath(pstPathLib);
    FREE(pstPathLib);

    const char* libFile = libPath.c_str();

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
                    const std::string & file = pMF->getPath();
                    std::size_t pos = file.find_last_of(L'.');
                    if (pos != std::string::npos)
                    {
                        instrumentMacro(module, file.substr(0, pos) + ".sci", macro);
                    }
                    else
                    {
                        instrumentMacro(module, file, macro);
                    }
                }
            }
        }
    }
}

void CoverModule::getBuiltins(const std::vector<std::pair<std::string, std::string>> & paths_mods)
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

void CoverModule::instrumentMacro(const std::string & module, const std::string & path, types::Macro * macro)
{
    const std::map<symbol::Variable *, types::Macro *> & submacros = macro->getSubMacros();
    for (const auto & p : submacros)
    {
        instrumentSingleMacro(module, path, p.second, true);
    }

    instrumentSingleMacro(module, path, macro, true);
}

void CoverModule::instrumentSingleMacro(const std::string & module, const std::string & path, types::Macro * macro, bool instrumentInners)
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

bool CoverModule::getStringFromXPath(const char * filePath, const char * xpquery, std::unordered_set<std::string> & set)
{
    xmlDocPtr doc = xmlReadFile(filePath, "utf-8", XML_PARSE_NOWARNING);
    if (!doc)
    {
        return false;
    }

    if (!doc->encoding || os_stricmp((const char *)doc->encoding, "utf-8") != 0)
    {
        xmlFreeDoc(doc);
        return false;
    }

    xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
    xmlXPathObjectPtr xp = xmlXPathEval((const xmlChar *)xpquery, ctxt);
    xmlNodeSetPtr nodeSet = xp->nodesetval;

    if (nodeSet)
    {
        for (int i = 0; i < nodeSet->nodeNr; ++i)
        {
            const char * content = (const char *)xmlNodeGetContent(nodeSet->nodeTab[i]);
            set.emplace(content);
            xmlFree(const_cast<char *>(content));
        }
    }

    xmlXPathFreeObject(xp);
    xmlXPathFreeContext(ctxt);
    xmlFreeDoc(doc);

    return nodeSet != nullptr;
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
                const std::string & name = current->getName();
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
    /*std::wcerr << "Builtin calls" << std::endl << tools::getUnderline(L"Builtin calls") << std::endl;
      for (const auto & p : callCounters)
      {
      if (p.second.get())
      {
      std::wcerr << p.first->getName() << " called " << p.second.get() << " time" << tools::getPlural(p.second.get()) << "." << std::endl;
      }
      }

      std::wcerr << std::endl;*/

    /*      for (const auto & counter : counters)
            {
            if (counter.get())
            {

            const std::string & name = counter.getMacro()->getName();
            std::wcerr << "Macro " << name << " visited at " << counter.getExp()->getLocation() << std::endl;


            //<< "Seq not visited:" << std::endl;

            }
            }
            */
    //collect();
}

void CoverModule::toHTML(const std::string & outputDir)
{
    bool nomodules = false;
    char * _outputDir = expandPathVariable(outputDir.c_str());
    createdirectory(_outputDir);

    if (results.size() == 1 && results.begin()->first == "" && results.begin()->second.size() == 1 && results.begin()->second.begin()->first == "")
    {
        for (const auto & p : macros)
        {
            writeMacroHTMLReport(p.first, results.begin()->second.begin()->second, _outputDir);
        }
        nomodules = true;
    }

    if (!nomodules)
    {
        std::map<std::string, std::pair<uint64_t, double>> modulesStats;

        // We make all the reports for the macros
        for (auto & p1 : results)
        {
            const std::string & moduleName = p1.first;
            const std::string __outputDir = std::string(_outputDir) + DIR_SEPARATOR + moduleName;
            createdirectory(__outputDir.c_str());
            uint64_t totalCalls = 0;
            uint64_t totalInstrs = 0;
            uint64_t uncoveredInstrs = 0;

            for (auto & p2 : p1.second)
            {
                writeMacroHTMLReport(p2.first, moduleName, p2.second, __outputDir);
            }

            // Now we make the module's reports
            std::ostringstream out;

            out << "<html lang=\'en\'>\n"
                << "<meta charset=\'UTF-8\'>\n"
                << "<head>\n"
                << "<link rel=\'icon\' href=\'../favicon.ico\'/>\n"
                << "<title>Scilab | Module " << moduleName << " | Scilab\'s code coverage" << "</title>\n"
                << "<style type=\'text/css\' media=\'all\'>\n"
                << "@import url(\'../mod_style.css\');\n"
                << "</style>\n"
                << "<script type=\'text/javascript\' src=\'../module.js\'></script>"
                << "</head>\n"
                << "<body>\n";

            bool altern = false;
            unsigned int tableid = 0;
            unsigned int buttonid = 0;

            out << "<h2 class=\'title\'>Macros calls</h2>\n"
                << "<table class='module'>\n"
                << "<tr><td><div class=\'macros_stats\'>\n"
                << "<div class=\'macros_cell\'><table id=\'table" << tableid << "\'>\n"
                << "<tr class=\'col_name\'><td>Name&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid, 0, false) << "</td><td>File</td><td>Calls&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 1, 2, true) << "</td><td>Covered&nbsp" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 2, 3, false) << "</td></tr>\n";

            for (const auto & t : getOrderedResults(moduleName))
            {
                const CoverResult & res = *std::get<0>(t);
                const std::string & macroFilename = *std::get<1>(t);
                const std::string & macroName = *std::get<2>(t);

                totalInstrs += res.getInfo().instrsCount;
                uncoveredInstrs += res.getUncInstrs();

                std::size_t pos = macroFilename.find_last_of(L'.');
                std::string filename = macroFilename.substr(0, pos);
                pos = filename.find_last_of("\\/");
                if (pos != std::string::npos)
                {
                    filename = filename.substr(pos + 1);
                }
                std::string filepath = encodeFilename(filename);

                const std::string countercls = res.counter == 0 ? "null_stats" : "stats";
                const std::string trcls = altern ? "altern1" : "altern2";
                out << "<tr class=\'" << trcls << "\'><td><span class=\'cmp\'>" << macroName << "</span></td>"
                    << "<td><a class=\'filepath' href=\'" << filepath << ".html\'>" << moduleName << "/macros/" << filename << ".sci</a></td>"
                    << "<td class=\'" << countercls << "\'><span class=\'cmp\'>" << res.counter << "</span></td>"
                    << "<td><span class=\'inline_percent\'><span class=\'cmp\'>" << res.getCovInstrsPercent() << "</span> %</span>";
                CovHTMLCodePrinter::getDivPercent(out, res.getCovInstrsPercent());
                out << "</td></tr>\n";

                altern = !altern;
            }

            ++tableid;
            buttonid += 3;

            out << "</table></div></div></td></tr>\n"
                << "</table>\n";

            // Now we print the number of calls for builtins.

            altern = false;

            out << "<h2 class=\'title\'>Builtins calls</h2>\n"
                << "<table class='module'>\n"
                << "<tr><td><div class='modulePath'>" << moduleName << "</div></td></tr>\n"
                << "<tr><td><div class=\'builtins_stats\'>\n"
                << "<div class=\'builtins_cell\'><table id=\'table" << tableid << "\'>\n"
                << "<tr class=\'col_name\'><td>Name&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid, 0, false) << "</td><td class=\'col_name\'>Calls&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 1, 1, true) << "</td></tr>\n";

            for (const auto & p : getBuiltinStats(moduleName))
            {
                const std::string countercls = p.second == 0 ? "null_stats" : "stats";
                const std::string trcls = altern ? "altern1" : "altern2";
                out << "<tr class=\'" << trcls << "\'><td><span class=\'cmp\'>" << *p.first << "</span></td><td class=\'" << countercls << "\'><span class=\'cmp\'>" << p.second << "</span></td></tr>\n";
                altern = !altern;
                totalCalls += p.second;
            }

            out << "</table></div>\n"
                << "</div></td></tr>\n"
                << "</table>\n"
                << "</body>\n"
                << "</html>\n";

            out.flush();
            writeFile(out, __outputDir, moduleName + ".html");

            const double percent = totalInstrs ? std::round(100. * (1. - (double)uncoveredInstrs / (double)totalInstrs)) : 100.;
            modulesStats.emplace(moduleName, std::pair<uint64_t, double>(totalCalls, percent));
        }

        // Now we must output the results for all the modules
        const std::string __outputDir(_outputDir);
        std::ostringstream out;

        out << "<html lang=\'en\'>\n"
            << "<meta charset=\'UTF-8\'>\n"
            << "<head>\n"
            << "<link rel=\'icon\' href=\'favicon.ico\'/>\n"
            << "<title>Scilab\'s code coverage" << "</title>\n"
            << "<style type=\'text/css\' media=\'all\'>\n"
            << "@import url(\'mod_style.css\');\n"
            << "</style>\n"
            << "<script type=\'text/javascript\' src=\'module.js\'></script>"
            << "</head>\n"
            << "<body>\n";

        bool altern = false;
        unsigned int tableid = 0;
        unsigned int buttonid = 0;

        out << "<h2 class=\'title\'>Modules results</h2>\n"
            << "<table class='module'>\n"
            << "<tr><td><div class=\'macros_stats\'>\n"
            << "<div class=\'macros_cell\'><table id=\'table" << tableid << "\'>\n"
            << "<tr class=\'col_name\'><td>Name&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid, 0, false) << "</td><td>Builtin calls&nbsp;" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 1, 1, true) << "</td><td>Covered&nbsp" << CovHTMLCodePrinter::getOrderButton(tableid, buttonid + 2, 2, false) << "</td></tr>\n";

        for (const auto & p : modulesStats)
        {
            const std::string & moduleName = p.first;
            const uint64_t builtinCalls = p.second.first;
            const double percent = p.second.second;
            const std::string countercls = builtinCalls ? "stats" : "null_stats";
            const std::string trcls = altern ? "altern1" : "altern2";
            out << "<tr class=\'" << trcls << "\'>"
                << "<td><span class=\'cmp\'><a class=\'filepath' href=\'" << moduleName << "/" << moduleName << ".html\'>" << moduleName << "</a></span></td>"
                << "<td class=\'" << countercls << "\'><span class=\'cmp\'>" << builtinCalls << "</span></td>"
                << "<td><span class=\'inline_percent\'><span class=\'cmp\'>" << percent << "</span> %</span>";

            CovHTMLCodePrinter::getDivPercent(out, (unsigned int)percent);
            out << "</tr>\n";
            altern = !altern;
        }

        out << "</table></div>\n"
            << "</div></td></tr>\n"
            << "</table>\n"
            << "</body>\n"
            << "</html>\n";

        out.flush();
        writeFile(out, __outputDir, "report.html");
    }

    FREE(_outputDir);
    copyDataFiles(outputDir);
}

void CoverModule::copyDataFiles(const std::string & outputDir)
{
    const std::string _outputDir = outputDir + DIR_SEPARATOR;
    const std::string _inputDir = std::string("SCI") + DIR_SEPARATOR + "modules" + DIR_SEPARATOR + "coverage" + DIR_SEPARATOR + "data";
    copyFile(_inputDir, _outputDir, "scilab_code.css");
    copyFile(_inputDir, _outputDir, "src_style.css");
    copyFile(_inputDir, _outputDir, "mod_style.css");
    copyFile(_inputDir, _outputDir, "favicon.ico");
    copyFile(_inputDir, _outputDir, "module.js");
}

void CoverModule::copyFile(const std::string & inDir, const std::string & outDir, const std::string & filename)
{
    const std::string in = inDir + DIR_SEPARATOR + filename;
    const std::string out = outDir + DIR_SEPARATOR + filename;
    char * _input = expandPathVariable(in.c_str());
    char * _output = expandPathVariable(out.c_str());
    CopyFileFunction(_output, _input);
    FREE(_input);
    FREE(_output);
}

void CoverModule::writeFile(const std::ostringstream & out, const std::string & outputDir, const std::string & filename)
{
    const std::string code = out.str();
    const std::string _filename = outputDir + DIR_SEPARATOR + filename;
    std::fstream file(_filename, std::ios::out | std::ios::trunc);
    file.write(code.c_str(), code.size());
    file.close();
}

std::set<CoverModule::__Res1, CoverModule::__Compare1> CoverModule::getOrderedResults(const std::string & moduleName) const
{
    std::set<__Res1, __Compare1> set;
    auto i = results.find(moduleName);
    if (i != results.end())
    {
        for (const auto & p : i->second)
        {
            const std::string & macroFilename = p.first;
            for (const auto & pp : p.second)
            {
                const std::string & macroName = pp.first.name;
                const Location & macroLoc = pp.first.loc;
                const CoverResult & res = pp.second;
                set.emplace(__Res1(&res, &macroFilename, &macroName, &macroLoc));
            }
        }
    }

    return set;
}

std::set<CoverModule::__Res2, CoverModule::__Compare2> CoverModule::getBuiltinStats(const std::string & moduleName) const
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

std::vector<std::pair<types::Callable *, uint64_t>> CoverModule::getFunctionCalls(const std::string & moduleName, const bool builtin) const
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

ast::Exp * CoverModule::getTree(const std::string & path)
{
    if (!path.empty())
    {
        std::ifstream src(path, std::ios::in | std::ios::binary | std::ios::ate);
        if (src.is_open())
        {
            src.seekg(0, src.end);
            int len = (int)src.tellg();
            src.seekg(0, src.beg);
            char * buffer = new char[len + 1];
            buffer[len] = '\0';
            src.read(buffer, len);
            src.close();

            Parser parser;
            parser.parse(buffer);
            delete[] buffer;

            return parser.getTree();
        }
    }
    return nullptr;
}

const std::string CoverModule::getName(const std::string & path)
{
    std::size_t pos = path.find_last_of(L'.');
    std::string name = path.substr(0, pos);
    pos = name.find_last_of("\\/");
    if (pos != std::string::npos)
    {
        name = name.substr(pos + 1);
    }
    return name;
}

void CoverModule::writeMacroHTMLReport(types::Macro * macro, std::map<MacroLoc, CoverResult> & results, const std::string & outputDir)
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

    writeMacroHTMLReport(fdec, macro->getName() + ".html", "", "", results, outputDir);
}

void CoverModule::writeMacroHTMLReport(ast::Exp * tree, const std::string & filename, const std::string & path, const std::string & moduleName, std::map<MacroLoc, CoverResult> & results, const std::string & outputDir)
{
    std::ostringstream out;
    std::string mod, prev;
    if (!moduleName.empty())
    {
        mod = " | Module " + moduleName;
        prev = "../";
    }
    std::string pa;
    if (!path.empty())
    {
        pa = " | " + path;
    }

    out << "<html lang=\'en\'>\n"
        << "<meta charset=\'UTF-8\'>\n"
        << "<head>\n"
        << "<link rel=\'icon\' href=\'../favicon.ico\'/>\n"
        << "<title>Scilab" << mod << pa << " | Scilab\'s code coverage" << "</title>\n"
        << "<style type=\'text/css\' media=\'all\'>\n"
        << "@import url(\'" << prev << "scilab_code.css\');\n"
        << "@import url(\'" << prev << "src_style.css\');\n"
        << "</style>\n"
        << "<script>\n"
        << "function show(did,fid) {\n"
        << "  x = document.getElementById(did).style;\n"
        << "  y = document.getElementById(fid);\n"
        << "  x.visibility = 'visible';\n"
        << "  x.display = 'block';\n"
        << "  x.height = 'auto';\n"
        << "  x.left = y.offsetLeft + 'px';\n"
        << "  x.top = y.offsetTop + y.offsetHeight + 'px';\n"
        << "}\n"
        << "function hide(did) {\n"
        << "  document.getElementById(did).style.visibility = \'hidden\';\n"
        << "}\n"
        << "</script>\n"
        << "</head>\n"
        << "<body>\n"
        << "<h2 class=\'title\'>Coverage and Profiling report</h2>\n"
        << "<table class='sourceFile'>\n";

    if (!path.empty())
    {
        out << "<tr><td><div class='sourcePath'>" << path << "</div></td></tr>\n";
    }
    if (!results.empty())
    {
        for (const auto & p : results)
        {
            out << "<tr><td><div class=\'allmacstats\'>\n";
            CovHTMLCodePrinter::getFunctionStats(out, p.first, p.second);
            out << "</div></td></tr\n";
        }
    }

    out << "<tr><td><div class=\'source\'>\n"
        << "<table>\n";

    CovHTMLCodePrinter printer(out, results);
    CodePrinterVisitor visitor(printer);
    tree->accept(visitor);
    printer.close();
    delete tree;

    out << "</table>\n"
        << "</div></td></tr>\n"
        << "</table>\n"
        << "</body>\n"
        << "</html>\n";

    out.flush();
    writeFile(out, outputDir, filename);
}

bool CoverModule::writeMacroHTMLReport(const std::string & path, const std::string & moduleName, std::map<MacroLoc, CoverResult> & results, const std::string & outputDir)
{
    if (ast::Exp * tree = getTree(path))
    {
        writeMacroHTMLReport(tree, getName(path) + ".html", path, moduleName, results, outputDir);
        return true;
    }

    return false;
}

std::string CoverModule::encodeFilename(const std::string & name)
{
    std::ostringstream wos;
    for (const auto c : name)
    {
        URLEncoder::replace(wos, c);
    }
    return wos.str();
}

void CoverModule::merge(const std::vector<std::string> & paths, const std::string & out)
{
    CoverModule cm;
    for (const auto path : paths)
    {
        cm.load(path);
    }
    cm.save(out);
}

void CoverModule::toHTML(const std::string & inBin, const std::string & outDir)
{
    CoverModule cm;
    cm.load(inBin);
    cm.toHTML(outDir);
}

void CoverModule::save(const std::string & path) const
{
    if (!path.empty())
    {
        std::fstream out(path, std::ios::out | std::ios::binary);
        if (out.is_open())
        {
            toBin(out);
            out.close();
        }
    }
}

void CoverModule::load(const std::string & path)
{
    if (!path.empty())
    {
        std::fstream in(path, std::ios::in | std::ios::binary);
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
        const std::string moduleName = CoverModule::readString(in);
        const uint64_t sizee = CoverModule::readUint64_t(in);
        auto it = cm.results.find(moduleName);
        if (it == cm.results.end())
        {
            it = cm.results.emplace(moduleName, std::unordered_map<std::string, std::map<MacroLoc, CoverResult>>()).first;
        }
        auto & map = it->second;
        for (uint64_t j = 0; j < sizee; ++j)
        {
            const std::string macroFilename = CoverModule::readString(in);
            const uint64_t sizeee = CoverModule::readUint64_t(in);
            auto it = map.find(macroFilename);
            if (it == map.end())
            {
                it = map.emplace(macroFilename, std::map<MacroLoc, CoverResult>()).first;
            }
            auto & mapp = it->second;
            for (uint64_t k = 0; k < sizeee; ++k)
            {
                const std::string macroName = CoverModule::readString(in);
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
        const std::string moduleName = CoverModule::readString(in);
        const uint64_t sizee = CoverModule::readUint64_t(in);
        auto it = cm.allCounters.find(moduleName);
        if (it == cm.allCounters.end())
        {
            it = cm.allCounters.emplace(moduleName, std::unordered_map<std::string, std::pair<bool, uint64_t>>()).first;
        }
        auto & map = it->second;
        for (uint64_t j = 0; j < sizee; ++j)
        {
            const std::string funName = CoverModule::readString(in);
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
