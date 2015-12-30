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
                branches.emplace(parent->getLocation(), std::vector<uint64_t>(1, i->get()));
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

std::ostream & operator<<(std::ostream & out, const CoverResult & cr)
{
    const std::size_t instrs = cr.info.instrsCount;
    const double percentInstrs = instrs ? 100 * (1 - (double)cr.uncoveredInstrs / (double)instrs) : 100;
    const std::size_t branches = cr.info.branchesCount;
    const double percentBranches = branches ? 100 * (1 - (double)cr.uncoveredBranches / (double)branches) : 100.;
    out << tools::underline("Macro " + cr.name)
        << " -called: " << cr.counter << " time" << tools::getPlural(cr.counter) << "." << std::endl
        << " -instrs: " << instrs << std::endl
        << " -branches: " << cr.info.branchesCount << std::endl
        << " -paths: " << cr.info.pathsCount << std::endl
        << " -covered instrs: " << (instrs - cr.uncoveredInstrs) << " (" << percentInstrs << "%)" << std::endl
        << " -covered branches: " << (branches - cr.uncoveredBranches) << " (" << percentBranches << "%)" << std::endl
        << std::endl;

    if (!cr.branches.empty())
    {
        out << tools::underline("Branches stats");
        for (const auto & p : cr.branches)
        {
            const std::vector<uint64_t> & counters = p.second;
            const std::size_t size = counters.size();
            out << "Expr at " << p.first << " has " << size << (size > 1 ? " branches" : " branch") << std::endl;

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
                        out << "Br " << (i + 1) << ": " << percent << "% ";
                    }
                    out << std::endl;
                }
            }
        }

        out << std::endl;
    }

    if (cr.counter && !cr.unused.empty())
    {
        out << tools::underline("Non covered") << std::endl;

        ast::PrintVisitor print(out);
        for (const auto & loc : cr.unused)
        {
            out << "At " << loc << ":" << std::endl;
            //e->accept(print);
            out << std::endl;
        }
    }

    return out;
}

void CoverResult::toXML(const std::string & outputDir)
{
    std::ostringstream out;
    const std::string tab1("  ");
    const std::string tab2("    ");
    const std::string tab3("      ");
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << std::endl

        << "<cover name=\"" << name << "\""
        << " file=\"" << info.macroFilePath << "\""
        << " module=\"" << info.macroModule << "\""
        << " instrs_count=\"" << info.instrsCount << "\""
        << " branches_count=\"" << info.branchesCount << "\""
        << " paths_count=\"" << info.pathsCount << "\""
        << ">" << std::endl

        << tab1 << "<result instrs_count=\"" << (info.instrsCount - uncoveredInstrs) << "\""
        << " branches_count=\"" << (info.branchesCount - uncoveredBranches) << "\""
        << " paths_count=\"" << 0 << "\""
        << "/>" << std::endl;

    if (branches.empty())
    {
        out << tab1 << "<branches/>" << std::endl;
    }
    else
    {
        out << tab1 << "<branches>" << std::endl;
        for (const auto & p : branches)
        {
            const std::vector<uint64_t> & counters = p.second;
            const std::size_t size = counters.size();
            const Location & loc = p.first;
            out << tab2 << "<exp branches_number=\"" << size << "\">" << std::endl

                << tab3 << "<location first_line=\"" << loc.first_line << "\""
                << " first_column=\"" << loc.first_column << "\""
                << " last_line=\"" << loc.last_line << "\""
                << " last_column=\"" << loc.last_column << "\""
                << "/>" << std::endl;

            for (std::size_t i = 0; i < size; ++i)
            {
                out << tab3 << "<branche index=\"" << (i + 1) << "\""
                    << " counter=\"" << counters[i] << "\""
                    << "/>" << std::endl;
            }

            out << tab2 << "</exp>" << std::endl;
        }
        out << tab1 << "</branches>" << std::endl;
    }

    if (unused.empty())
    {
        out << tab1 << "<uncovered/>" << std::endl;
    }
    else
    {
        out << tab1 << "<uncovered>" << std::endl;
        for (const auto & loc : unused)
        {
            out << tab2 << "<location first_line=\"" << loc.first_line << "\""
                << " first_column=\"" << loc.first_column << "\""
                << " last_line=\"" << loc.last_line << "\""
                << " last_column=\"" << loc.last_column << "\""
                << "/>" << std::endl;
        }
        out << tab1 << "</uncovered>" << std::endl;
    }

    out << "</cover>";

    const char * code = out.str().c_str();
    char * _output = expandPathVariable((char*)outputDir.c_str());
    const std::string filename = std::string(_output) + DIR_SEPARATOR + name + ".xml";
    std::fstream file(filename, std::ios::out | std::ios::trunc);
    file.write(code, std::strlen(code));
    file.close();
    FREE(_output);
}

void CoverResult::toJSON(const std::string & outputDir)
{
    std::ostringstream out;
    const std::string tab1("    ");
    const std::string tab2("        ");
    const std::string tab3("            ");
    out << "{" << std::endl

        << tab1 << "\"name\": \"" << name << "\"," << std::endl
        << tab1 << "\"file\": \"" << info.macroFilePath << "\"," << std::endl
        << tab1 << "\"module\": \"" << info.macroModule << "\"," << std::endl
        << tab1 << "\"instrs_count\": \"" << info.instrsCount << "\"," << std::endl
        << tab1 << "\"branches_count\": \"" << info.branchesCount << "\"," << std::endl
        << tab1 << "\"paths_count\": \"" << info.pathsCount << "\"," << std::endl

        << tab1 << "\"result\": {" << std::endl
        << tab2 << "\"instrs_count\": \"" << (info.instrsCount - uncoveredInstrs) << "\"," << std::endl
        << tab2 << "\"branches_count\": \"" << (info.branchesCount - uncoveredBranches) << "\"," << std::endl
        << tab2 << "\"paths_count\": \"" << 0 << "\"," << std::endl
        << tab1 << "}," << std::endl;

    if (branches.empty())
    {
        out << tab1 << "\"branches\": []," << std::endl;
    }
    else
    {
        out << tab1 << "\"branches\": [" << std::endl;
        const auto last = std::prev(branches.end());
        for (auto i = branches.begin(), end = branches.end(); i != end; ++i)
        {
            const std::vector<uint64_t> & counters = i->second;
            const std::size_t size = counters.size();
            const Location & loc = i->first;
            out << tab2 << "{" << std::endl
                << tab3 << "\"location\": { \"first_line\": \"" << loc.first_line << "\""
                << ", \"first_column\": \"" << loc.first_column << "\""
                << ", \"last_line\": \"" << loc.last_line << "\""
                << ", \"last_column\": \"" << loc.last_column << "\""
                << "}," << std::endl
                << tab3 << "\"counters\": [";

            for (std::size_t j = 0; j < size - 1; ++j)
            {
                out << "\"" << counters[j] << "\", ";
            }
            out << "\"" << counters[size - 1] << "\"]" << std::endl;

            if (i != last)
            {
                out << tab2 << "}, " << std::endl;
            }
            else
            {
                out << tab2 << "}" << std::endl;
            }
        }
        out << tab1 << "]," << std::endl;
    }

    if (unused.empty())
    {
        out << tab1 << "\"uncovered\": []" << std::endl;
    }
    else
    {
        out << tab1 << "\"uncovered\": [" << std::endl;
        const auto last = std::prev(unused.end());
        for (auto i = unused.begin(), end = unused.end(); i != end; ++i)
        {
            const Location & loc = *i;
            out << tab2 << "{ \"first_line\": \"" << loc.first_line << "\""
                << ", \"first_column\": \"" << loc.first_column << "\""
                << ", \"last_line\": \"" << loc.last_line << "\""
                << ", \"last_column\": \"" << loc.last_column << "\"";
            if (i != last)
            {
                out << "}," << std::endl;
            }
            else
            {
                out << "}" << std::endl;
            }
        }
        out << tab1 << "]" << std::endl;
    }

    out << "}";

    const char * code = out.str().c_str();
    const std::string filename = out.str() + DIR_SEPARATOR + name + ".json";
    std::fstream file(filename, std::ios::out | std::ios::trunc);
    file.write(code, std::strlen(code));
    file.close();
}

void CoverResult::toHTML(const std::string & outputDir)
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

      out << "<html lang=\'en\'>\n"
      << "<meta charset=\'UTF-8\'>\n"
      << "<head>\n"
      << "<title>Scilab\'s coverage - " << info.macroModule << "/" << info.macroFilePath << "</title>\n"
      << "<style type=\'text/css\' media=\'all\'>\n"
      << "@import url(\'./scilab_code.css\');\n"
      << "@import url(\'./style.css\');\n"
      << "</style>\n"
      << "</head>\n"
      << "<body>\n"
      << "<div class=\'source\'>\n"
      << "<table>\n";

      HTMLCodePrinter printer(out, this);
      CodePrinterVisitor visitor(printer);
      tree->accept(visitor);
      printer.close();
      delete tree;

      out << "</table>\n"
      << "</div>\n"
      << "</body>\n"
      << "</html>\n";

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
    const std::string name = CoverModule::readString(in);
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

std::string CoverResult::getStringTime(const uint64_t time) const
{
    if (time == 0)
    {
        return "0 s";
    }
    else if (time < 1000000UL)
    {
        // between 0 and 10^6 ns
        std::ostringstream out;
        out << ((double)time / 1000.) << " µs";
        return out.str();
    }
    else if (time < 1000000000UL)
    {
        // between 0 and 10^9 ns
        std::ostringstream out;
        out << ((double)(time / 1000UL) / 1000.) << " ms";
        return out.str();
    }
    else
    {
        std::ostringstream out;
        out << ((double)(time / 1000000UL) / 1000.) << " s";
        return out.str();
    }
}
}
