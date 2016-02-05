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

#ifndef __COVER_MODULE_HXX__
#define __COVER_MODULE_HXX__

#include <cstring>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Counter.hxx"
#include "InstrumentVisitor.hxx"
#include "CoverMacroInfo.hxx"
#include "CoverResult.hxx"
#include "MacroLoc.hxx"
#include "dynlib_coverage.h"
#include "UTF8.hxx"

namespace coverage
{

class COVERAGE_IMPEXP CoverModule
{

    std::vector<Counter> counters;
    std::unordered_map<types::Callable *, CallCounter> callCounters;
    std::unordered_map<types::Macro *, CoverMacroInfo> macros;

    // { moduleName => functions }
    std::unordered_multimap<std::wstring, types::Callable *> functions;

    // { moduleName => { macroFilename => { macroName => CoverResult } } }
    std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::map<MacroLoc, CoverResult>>> results;
    // { moduleName => { funname => <ismacro, counter> } }
    std::unordered_map<std::wstring, std::unordered_map<std::wstring, std::pair<bool, uint64_t>>> allCounters;

    InstrumentVisitor visitor;

    static CoverModule * instance;

    CoverModule(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods);
    CoverModule(const std::vector<std::wstring> & moduleNames);
    CoverModule();

    ~CoverModule();

public:

    inline static CoverModule * createInstance(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods)
    {
        delete instance;
        instance = new CoverModule(paths_mods);
        return instance;
    }

    inline static CoverModule * createInstance(const std::vector<std::wstring> & moduleNames)
    {
        delete instance;
        instance = new CoverModule(moduleNames);
        return instance;
    }

    inline static CoverModule * createInstance()
    {
        delete instance;
        instance = new CoverModule();
        return instance;
    }

    inline static CoverModule * getInstance()
    {
        return instance;
    }

    inline static void clearInstance()
    {
        delete instance;
        instance = nullptr;
    }

    void add(types::Macro * macro, ast::Exp * e);
    void invoke(types::Callable * f);
    void invoke(const uint64_t id);
    void print();
    void collect();
    void toHTML(const std::wstring & outputDir);
    void toBin(std::fstream & out) const;
    void save(const std::wstring & path) const;
    void load(const std::wstring & path);

    void fromBin(std::fstream & in);
    static void fromBin(CoverModule & cm, std::fstream & in);
    static void merge(const std::vector<std::wstring> & paths, const std::wstring & out);
    static void toHTML(const std::wstring & inBin, const std::wstring & outDir);

    inline bool isCovered(types::Callable * f) const
    {
        return callCounters.find(f) != callCounters.end();
    }

    inline void startChrono(const uint64_t id)
    {
        counters[id - 2].startChrono();
    }

    inline void stopChrono(const uint64_t id)
    {
        counters[id - 2].stopChrono();
    }

    inline static void invoke(const ast::Exp & e)
    {
        if (instance && e.getCoverId())
        {
            instance->invoke(e.getCoverId());
        }
    }

    inline static void invokeAndStartChrono(const ast::Exp & e)
    {
        if (instance && e.getCoverId())
        {
            instance->invoke(e.getCoverId());
            instance->startChrono(e.getCoverId());
        }
    }

    inline static void startChrono(const ast::Exp & e)
    {
        if (instance && e.getCoverId())
        {
            instance->startChrono(e.getCoverId());
        }
    }

    inline static void stopChrono(const ast::Exp & e)
    {
        if (instance && e.getCoverId())
        {
            instance->stopChrono(e.getCoverId());
        }
    }

    inline static void write(std::fstream & out, const std::wstring & str)
    {
        const std::string _str = scilab::UTF8::toUTF8(str);
        uint32_t n = _str.size();
        out.write((char *)&n, sizeof(uint32_t));
        out.write(_str.c_str(), sizeof(char) * n);
    }

    inline static void write(std::fstream & out, const uint64_t n)
    {
        out.write((char *)&n, sizeof(uint64_t));
    }

    inline static void write(std::fstream & out, const int32_t n)
    {
        out.write((char *)&n, sizeof(int32_t));
    }

    inline static void write(std::fstream & out, const bool n)
    {
        out.write((char *)&n, sizeof(bool));
    }

    inline static void write(std::fstream & out, const Location & loc)
    {
        write(out, loc.first_line);
        write(out, loc.first_column);
        write(out, loc.last_line);
        write(out, loc.last_column);
    }

    inline static std::wstring readWstring(std::fstream & in)
    {
        uint32_t n;
        in.read((char*)&n, sizeof(uint32_t));
        char * buf = new char[n + 1];
        buf[n] = '\0';
        in.read(buf, n * sizeof(char));
        wchar_t * wstr = to_wide_string(buf);
        std::wstring str(wstr);
        delete[] buf;
        FREE(wstr);

        return str;
    }

    inline static uint64_t readUint64_t(std::fstream & in)
    {
        uint64_t n;
        in.read((char *)&n, sizeof(uint64_t));

        return n;
    }

    inline static int32_t readInt32_t(std::fstream & in)
    {
        int32_t n;
        in.read((char *)&n, sizeof(int32_t));

        return n;
    }

    inline static bool readBool(std::fstream & in)
    {
        bool n;
        in.read((char *)&n, sizeof(bool));

        return n;
    }

    inline static Location readLocation(std::fstream & in)
    {
        const int32_t fl = readInt32_t(in);
        const int32_t fc = readInt32_t(in);
        const int32_t ll = readInt32_t(in);
        const int32_t lc = readInt32_t(in);

        return Location(fl, ll, fc, lc);
    }


    void instrumentSingleMacro(const std::wstring & module, const std::wstring & path, types::Macro * macro, bool instrumentInners);

private:

    void getMacros(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods);
    void getMacros(const std::wstring & path, const std::wstring & module);
    void getMacrosFromDir(const std::wstring & path, const std::wstring & module);
    void getBuiltins(const std::vector<std::pair<std::wstring, std::wstring>> & paths_mods);
    void instrumentMacro(const std::wstring & module, const std::wstring & path, types::Macro * macro);

    static bool getStringFromXPath(char * filePath, const char * xpquery, std::unordered_set<std::wstring> & set);
    static void copyDataFiles(const std::wstring & outputDir);
    static void copyFile(const std::wstring & inDir, const std::wstring & outDir, const std::wstring & filename);
    static void writeFile(const std::wostringstream & out, const std::wstring & outputDir, const std::wstring & filename);
    static ast::Exp * getTree(const std::wstring & path);
    static const std::wstring getName(const std::wstring & path);
    static void writeMacroHTMLReport(ast::Exp * tree, const std::wstring & filename, const std::wstring & path, const std::wstring & moduleName, std::map<MacroLoc, CoverResult> & results, const std::wstring & outputDir);
    static bool writeMacroHTMLReport(const std::wstring & path, const std::wstring & moduleName, std::map<MacroLoc, CoverResult> & results, const std::wstring & outputDir);
    static void writeMacroHTMLReport(types::Macro * macro, std::map<MacroLoc, CoverResult> & results, const std::wstring & outputDir);
    static std::wstring encodeFilename(const std::wstring & name);
    static const std::vector<std::pair<std::wstring, std::wstring>> getModule(const std::vector<std::wstring> & moduleNames);

    typedef std::tuple<const CoverResult *, const std::wstring *, const std::wstring *, const Location *> __Res1;
    struct __Compare1
    {
        inline bool operator()(const __Res1 & l, const __Res1 & r) const
        {
            return std::get<0>(l)->counter < std::get<0>(r)->counter || (std::get<0>(l)->counter == std::get<0>(r)->counter && (std::get<2>(l) < std::get<2>(r) || (std::get<2>(l) == std::get<2>(r) && std::get<3>(l) < std::get<3>(r))));
        }
    };
    std::set<__Res1, __Compare1> getOrderedResults(const std::wstring & moduleName) const;

    typedef std::pair<const std::wstring *, uint64_t> __Res2;
    struct __Compare2
    {
        inline bool operator()(const __Res2 & l, const __Res2 & r) const
        {
            return l.second < r.second || (l.second == r.second && *l.first < *r.first);
        }
    };
    std::set<__Res2, __Compare2> getBuiltinStats(const std::wstring & moduleName) const;

    std::vector<std::pair<types::Callable *, uint64_t>> getFunctionCalls(const std::wstring & moduleName, const bool builtin) const;

};

} // namespace coverage

#endif // __COVER_MODULE_HXX__
