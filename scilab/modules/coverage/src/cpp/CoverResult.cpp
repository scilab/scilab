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

#include "CoverModule.hxx"
#include "CoverResult.hxx"
#include "cover_tools.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alltypes.hxx"
#include "printvisitor.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "expandPathVariable.h"
}

namespace coverage
{

void CoverResult::populate(const std::vector<Counter>::const_iterator pos, const std::vector<Counter>::const_iterator end)
{
    Location current(0, 0, 0, 0), prev(0, 0, 0, 0);
    for (std::vector<Counter>::const_iterator i = pos; i != end; ++i)
    {
        ast::Exp * e = i->getExp();
        times.emplace(e->getLocation(), i->getNanoTime());

        if (!CoverResult::isInside(prev, e->getLocation()))
        {
            addNanoTime(i->getNanoTime());
            prev = e->getLocation();
        }

        if ((e->isSeqExp() || e->isCommentExp()) && e->getParent() && !e->getParent()->isFunctionDec())
        {
            // we have a branch with a parent
            ast::Exp * parent = e->getParent();
            auto j = branches.find(parent->getLocation());
            if (j == branches.end())
            {
                branches.emplace(parent->getLocation(), std::vector<uint64_t>({i->get(), 0}));
            }
            else
            {
                j->second.emplace_back(i->get());
            }
            if (parent->isForExp() || parent->isWhileExp())
            {
                loops.emplace(parent->getLocation(), i->get());
            }
        }

        if (i->get())
        {
            // Covered expression
        }
        else
        {
            // Uncovered expression
            if ((e->isSeqExp() || e->isCommentExp()) && e->getParent() && !e->getParent()->isFunctionDec())
            {
                ++uncoveredBranches;
            }
            else
            {
                ++uncoveredInstrs;
                if (!CoverResult::isInside(current, e->getLocation()))
                {
                    unused.insert(e->getLocation());
                    current = e->getLocation();
                }
            }
        }
    }
}

bool CoverResult::isCovered(const ast::Exp * e) const
{
    if (unused.empty())
    {
        return true;
    }
    auto i = unused.lower_bound(e->getLocation());
    if (i == unused.end())
    {
        return !CoverResult::isInside(*std::prev(i), e->getLocation());
    }
    else
    {
        if (i == unused.begin())
        {
            return !CoverResult::isInside(*i, e->getLocation());
        }
        return !CoverResult::isInside(*i, e->getLocation()) && !CoverResult::isInside(*std::prev(i), e->getLocation());
    }
}

std::wostream & operator<<(std::wostream & out, const CoverResult & cr)
{
    const std::size_t instrs = cr.info.instrsCount;
    const double percentInstrs = instrs ? 100 * (1 - (double)cr.uncoveredInstrs / (double)instrs) : 100;
    const std::size_t branches = cr.info.branchesCount;
    const double percentBranches = branches ? 100 * (1 - (double)cr.uncoveredBranches / (double)branches) : 100.;
    out << tools::underline(L"Macro " + cr.name)
        << L" -called: " << cr.counter << L" time" << tools::getPlural(cr.counter) << L"." << '\n'
        << L" -instrs: " << instrs << '\n'
        << L" -branches: " << cr.info.branchesCount << '\n'
        << L" -paths: " << cr.info.pathsCount << '\n'
        << L" -covered instrs: " << (instrs - cr.uncoveredInstrs) << L" (" << percentInstrs << L"%)" << '\n'
        << L" -covered branches: " << (branches - cr.uncoveredBranches) << L" (" << percentBranches << L"%)" << '\n'
        << '\n';

    if (!cr.branches.empty())
    {
        out << tools::underline(L"Branches stats");
        for (const auto & p : cr.branches)
        {
            const std::vector<uint64_t> & counters = p.second;
            const std::size_t size = counters.size();
            out << L"Expr at " << p.first << L" has " << size << (size > 1 ? L" branches" : L" branch") << '\n';

            if (size > 1)
            {
                uint64_t sum = 0;
                for (const auto & c : counters)
                {
                    sum += c;
                }

                if (sum)
                {
                    for (std::size_t i = 0; i < size; ++i)
                    {
                        const double percent = (100. * (double)counters[i] / (double)sum);
                        out << L"Br " << (i + 1) << L": " << percent << L"% ";
                    }
                    out << '\n';
                }
            }
        }

        out << '\n';
    }

    if (cr.counter && !cr.unused.empty())
    {
        out << tools::underline(L"Non covered") << '\n';

        ast::PrintVisitor print(out);
        for (const auto & loc : cr.unused)
        {
            out << L"At " << loc << L":" << '\n';
            //e->accept(print);
            out << '\n';
        }
    }

    return out;
}

void CoverResult::toXML(const std::wstring & outputDir)
{
    std::wostringstream out;
    const std::wstring tab1(L"  ");
    const std::wstring tab2(L"    ");
    const std::wstring tab3(L"      ");
    out << L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << '\n'

        << L"<cover name=\"" << name << L"\""
        << L" file=\"" << info.macroFilePath << L"\""
        << L" module=\"" << info.macroModule << L"\""
        << L" instrs_count=\"" << info.instrsCount << L"\""
        << L" branches_count=\"" << info.branchesCount << L"\""
        << L" paths_count=\"" << info.pathsCount << L"\""
        << L">" << '\n'

        << tab1 << L"<result instrs_count=\"" << (info.instrsCount - uncoveredInstrs) << L"\""
        << L" branches_count=\"" << (info.branchesCount - uncoveredBranches) << L"\""
        << L" paths_count=\"" << 0 << L"\""
        << L"/>" << '\n';

    if (branches.empty())
    {
        out << tab1 << L"<branches/>" << '\n';
    }
    else
    {
        out << tab1 << L"<branches>" << '\n';
        for (const auto & p : branches)
        {
            const std::vector<uint64_t> & counters = p.second;
            const std::size_t size = counters.size();
            const Location & loc = p.first;
            out << tab2 << L"<exp branches_number=\"" << size << L"\">" << '\n'

                << tab3 << L"<location first_line=\"" << loc.first_line << L"\""
                << L" first_column=\"" << loc.first_column << L"\""
                << L" last_line=\"" << loc.last_line << L"\""
                << L" last_column=\"" << loc.last_column << L"\""
                << L"/>" << '\n';

            for (std::size_t i = 0; i < size; ++i)
            {
                out << tab3 << L"<branche index=\"" << (i + 1) << L"\""
                    << L" counter=\"" << counters[i] << L"\""
                    << L"/>" << '\n';
            }

            out << tab2 << L"</exp>" << '\n';
        }
        out << tab1 << L"</branches>" << '\n';
    }

    if (unused.empty())
    {
        out << tab1 << L"<uncovered/>" << '\n';
    }
    else
    {
        out << tab1 << L"<uncovered>" << '\n';
        for (const auto & loc : unused)
        {
            out << tab2 << L"<location first_line=\"" << loc.first_line << L"\""
                << L" first_column=\"" << loc.first_column << L"\""
                << L" last_line=\"" << loc.last_line << L"\""
                << L" last_column=\"" << loc.last_column << L"\""
                << L"/>" << '\n';
        }
        out << tab1 << L"</uncovered>" << '\n';
    }

    out << L"</cover>";

    char * code = wide_string_to_UTF8(out.str().c_str());
    wchar_t * _output = expandPathVariableW((wchar_t *)outputDir.c_str());
    const std::wstring filename = std::wstring(_output) + DIR_SEPARATORW + name + L".xml";
    char * _filename = wide_string_to_UTF8(filename.c_str());
    std::fstream file(_filename, std::ios::out | std::ios::trunc);
    file.write(code, std::strlen(code));
    file.close();
    FREE(code);
    FREE(_filename);
    FREE(_output);
}

void CoverResult::toJSON(const std::wstring & outputDir)
{
    std::wostringstream out;
    const std::wstring tab1(L"    ");
    const std::wstring tab2(L"        ");
    const std::wstring tab3(L"            ");
    out << L"{" << '\n'

        << tab1 << L"\"name\": \"" << name << L"\"," << '\n'
        << tab1 << L"\"file\": \"" << info.macroFilePath << L"\"," << '\n'
        << tab1 << L"\"module\": \"" << info.macroModule << L"\"," << '\n'
        << tab1 << L"\"instrs_count\": \"" << info.instrsCount << L"\"," << '\n'
        << tab1 << L"\"branches_count\": \"" << info.branchesCount << L"\"," << '\n'
        << tab1 << L"\"paths_count\": \"" << info.pathsCount << L"\"," << '\n'

        << tab1 << L"\"result\": {" << '\n'
        << tab2 << L"\"instrs_count\": \"" << (info.instrsCount - uncoveredInstrs) << L"\"," << '\n'
        << tab2 << L"\"branches_count\": \"" << (info.branchesCount - uncoveredBranches) << L"\"," << '\n'
        << tab2 << L"\"paths_count\": \"" << 0 << L"\"," << '\n'
        << tab1 << L"}," << '\n';

    if (branches.empty())
    {
        out << tab1 << L"\"branches\": []," << '\n';
    }
    else
    {
        out << tab1 << L"\"branches\": [" << '\n';
        for (auto i = branches.begin(); i != branches.end(); ++i)
        {
            const std::vector<uint64_t> & counters = i->second;
            const std::size_t size = counters.size();
            const Location & loc = i->first;
            out << tab2 << L"{" << '\n'
                << tab3 << L"\"location\": { \"first_line\": \"" << loc.first_line << L"\""
                << L", \"first_column\": \"" << loc.first_column << L"\""
                << L", \"last_line\": \"" << loc.last_line << L"\""
                << L", \"last_column\": \"" << loc.last_column << L"\""
                << L"}," << '\n'
                << tab3 << L"\"counters\": [";

            for (std::size_t j = 0; j < size - 1; ++j)
            {
                out << L"\"" << counters[j] << L"\", ";
            }
            out << L"\"" << counters[size - 1] << L"\"]" << '\n';

            out << tab2 << L"}, " << '\n';
        }
        out.seekp(-1, out.cur); // Ignore the last comma. Safe because we overwrite it with the next instruction
        out << tab1 << L"]," << '\n';
    }

    if (unused.empty())
    {
        out << tab1 << L"\"uncovered\": []" << '\n';
    }
    else
    {
        out << tab1 << L"\"uncovered\": [" << '\n';
        for (auto i = unused.begin(); i != unused.end(); ++i)
        {
            const Location & loc = *i;
            out << tab2 << L"{ \"first_line\": \"" << loc.first_line << L"\""
                << L", \"first_column\": \"" << loc.first_column << L"\""
                << L", \"last_line\": \"" << loc.last_line << L"\""
                << L", \"last_column\": \"" << loc.last_column << L"\"";
            out << L"}," << '\n';
        }
        out.seekp(-1, out.cur); // Ignore the last comma. Safe because we overwrite it with the next instruction
        out << tab1 << L"]" << '\n';
    }

    out << L"}";

    char * code = wide_string_to_UTF8(out.str().c_str());
    wchar_t * _output = expandPathVariableW((wchar_t *)outputDir.c_str());
    const std::wstring filename = std::wstring(_output) + DIR_SEPARATORW + name + L".json";
    char * _filename = wide_string_to_UTF8(filename.c_str());
    std::fstream file(_filename, std::ios::out | std::ios::trunc);
    file.write(code, std::strlen(code));
    file.close();
    FREE(code);
    FREE(_filename);
    FREE(_output);
}

void CoverResult::toHTML(const std::wstring & outputDir)
{
    /*
      char * _path = wide_string_to_UTF8(info.macroFilePath.c_str());
      std::ifstream src(_path, ios::in | ios::binary | ios::ate);
      FREE(_path);
      if (src.is_open())
      {
      src.seekg(0, src.end);
      int len = src.tellg();
      src.seekg (0, src.beg);
      char * buffer = new char[len + 1];
      buffer[len] = '\0';
      src.read(buffer, len);
      src.close();

      wchar_t * _wstr = to_wide_string(buffer);
      delete[] buffer;
      Parser parser;
      parser.parse(_wstr);
      FREE(_wstr);

      ast::Exp * tree = parser.getTree();
      std::wostringstream out;

      out << L"<html lang=\'en\'>\n"
      << L"<meta charset=\'UTF-8\'>\n"
      << L"<head>\n"
      << L"<title>Scilab\'s coverage - " << info.macroModule << L"/" << info.macroFilePath << L"</title>\n"
      << L"<style type=\'text/css\' media=\'all\'>\n"
      << L"@import url(\'./scilab_code.css\');\n"
      << L"@import url(\'./style.css\');\n"
      << L"</style>\n"
      << L"</head>\n"
      << L"<body>\n"
      << L"<div class=\'source\'>\n"
      << L"<table>\n";

      HTMLCodePrinter printer(out, this);
      CodePrinterVisitor visitor(printer);
      tree->accept(visitor);
      printer.close();
      delete tree;

      out << L"</table>\n"
      << L"</div>\n"
      << L"</body>\n"
      << L"</html>\n";

      out.flush();

      char * code = wide_string_to_UTF8(out.str().c_str());
      wchar_t * _output = expandPathVariableW((wchar_t *)outputDir.c_str());
      const std::wstring filename = std::wstring(_output) + DIR_SEPARATORW + name + L".html";
      char * _filename = wide_string_to_UTF8(filename.c_str());
      std::fstream file(_filename, std::ios::out | std::ios::trunc);
      file.write(code, std::strlen(code));
      file.close();
      FREE(code);
      FREE(_filename);
      FREE(_output);
      }
    */
}

void CoverResult::toBin(std::fstream & out) const
{
    CoverModule::write(out, name);
    info.toBin(out);
    CoverModule::write(out, uncoveredInstrs);
    CoverModule::write(out, uncoveredBranches);
    CoverModule::write(out, counter);
    CoverModule::write(out, time);

    CoverModule::write(out, (uint64_t)unused.size());
    for (const auto & loc : unused)
    {
        CoverModule::write(out, loc);
    }

    CoverModule::write(out, (uint64_t)branches.size());
    for (const auto & p : branches)
    {
        CoverModule::write(out, p.first);
        CoverModule::write(out, (uint64_t)p.second.size());
        for (const auto c : p.second)
        {
            CoverModule::write(out, c);
        }
    }

    CoverModule::write(out, (uint64_t)loops.size());
    for (const auto & p : loops)
    {
        CoverModule::write(out, p.first);
        CoverModule::write(out, p.second);
    }

    CoverModule::write(out, (uint64_t)times.size());
    for (const auto & p : times)
    {
        CoverModule::write(out, p.first);
        CoverModule::write(out, p.second);
    }

    out.flush();
}

CoverResult CoverResult::fromBin(std::fstream & in)
{
    const std::wstring name = CoverModule::readWstring(in);
    CoverMacroInfo info = CoverMacroInfo::fromBin(in);
    CoverResult res(name, info);
    res.uncoveredInstrs = CoverModule::readUint64_t(in);
    res.uncoveredBranches = CoverModule::readUint64_t(in);
    res.counter = CoverModule::readUint64_t(in);
    res.time = CoverModule::readUint64_t(in);
    uint64_t size = CoverModule::readUint64_t(in);
    for (uint64_t i = 0; i < size; ++i)
    {
        res.unused.emplace(CoverModule::readLocation(in));
    }
    size = CoverModule::readUint64_t(in);
    for (uint64_t i = 0; i < size; ++i)
    {
        std::vector<uint64_t> & v = res.branches.emplace(CoverModule::readLocation(in), std::vector<uint64_t>()).first->second;
        const uint64_t _size = CoverModule::readUint64_t(in);
        v.reserve(_size);
        for (uint64_t j = 0; j < _size; ++j)
        {
            v.emplace_back(CoverModule::readUint64_t(in));
        }
    }
    size = CoverModule::readUint64_t(in);
    for (uint64_t i = 0; i < size; ++i)
    {
        Location loc = CoverModule::readLocation(in);
        res.loops.emplace(loc, CoverModule::readUint64_t(in));
    }

    size = CoverModule::readUint64_t(in);
    for (uint64_t i = 0; i < size; ++i)
    {
        Location loc = CoverModule::readLocation(in);
        res.times.emplace(loc, CoverModule::readUint64_t(in));
    }

    return res;
}

void CoverResult::merge(const CoverResult & cr)
{
    uncoveredInstrs = std::min(uncoveredInstrs, cr.uncoveredInstrs);
    uncoveredBranches = std::min(uncoveredBranches, cr.uncoveredBranches);
    counter += cr.counter;
    time += cr.time;

    std::set<Location, __LocHelper::Compare> out;
    for (const auto & loc : unused)
    {
        auto i = cr.unused.find(loc);
        if (i != cr.unused.end())
        {
            out.emplace(loc);
        }
    }
    unused = out;

    for (auto & p : branches)
    {
        auto i = cr.branches.find(p.first);
        if (i != cr.branches.end())
        {
            // we iterate on the counters associated to each branch
            auto j = i->second.begin();
            for (auto & c : p.second)
            {
                c += *j;
                ++j;
            }
        }
    }

    for (auto & p : loops)
    {
        auto i = cr.loops.find(p.first);
        if (i != cr.loops.end())
        {
            p.second += i->second;
        }
    }

    for (auto & p : times)
    {
        auto i = cr.times.find(p.first);
        if (i != cr.times.end())
        {
            p.second += i->second;
        }
    }
}

std::wstring CoverResult::getStringTime(const uint64_t time) const
{
    if (time == 0)
    {
        return L"0 s";
    }
    else if (time < 1000000UL)
    {
        // between 0 and 10^6 ns
        std::wostringstream out;
        out << ((double)time / 1000.) << L" µs";
        return out.str();
    }
    else if (time < 1000000000UL)
    {
        // between 0 and 10^9 ns
        std::wostringstream out;
        out << ((double)(time / 1000UL) / 1000.) << L" ms";
        return out.str();
    }
    else
    {
        std::wostringstream out;
        out << ((double)(time / 1000000UL) / 1000.) << L" s";
        return out.str();
    }
}
}
