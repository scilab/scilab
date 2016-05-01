/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <chrono>
#include <ctime>
#include <string>

#include "output/cnes/CNESCsvResult.hxx"
#include "checkers/SLintChecker.hxx"
#include "FileException.hxx"
#include "SLint.hxx"
#include "SciFile.hxx"
#include "UTF8.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "charEncoding.h"
#include "localization.h"
}

namespace slint
{

namespace CNES
{

CNESCsvResult::CNESCsvResult(const ToolConfiguration & tc, types::String * conf, const std::wstring & id, const std::wstring & _path) : tct(tc.getToolConfiguration()), current(nullptr), path(_path)
{
    std::string projectName;
    std::string projectVersion;
    std::string projectAuthor;
    if (conf)
    {
        if (conf->getSize() >= 4)
        {
            projectName = getStr(conf->get(3));
        }
        if (conf->getSize() >= 5)
        {
            projectVersion = getStr(conf->get(4));
        }
        if (conf->getSize() >= 6)
        {
            projectAuthor = getStr(conf->get(5));
        }
    }
    const std::wstring fullpath = SLint::getFullPath(path);
    out = new std::ofstream(scilab::UTF8::toUTF8(fullpath), std::ios::out);
    if (!out->is_open())
    {
        delete out;
        out = nullptr;
        throw slint::FileException(fullpath, _("Cannot open the file"));
    }
    else
    {
        (*out) << projectName << ',';
        (*out) << projectVersion << ',';
        (*out) << projectAuthor << ',';
        (*out) << "scilab,";

        std::chrono::time_point<std::chrono::system_clock> point = std::chrono::system_clock::now();;
        std::time_t time = std::chrono::system_clock::to_time_t(point);
        std::string date(std::ctime(&time));
        if (date.back() == '\n')
        {
            date = std::string(date.begin(), std::prev(date.end()));
        }
        (*out) << getStr(date) << ",\n";
    }
}

CNESCsvResult::~CNESCsvResult()
{
    if (out)
    {
        out->close();
        delete out;
    }
}

void CNESCsvResult::finalize()
{
    printRes();
    out->close();
    delete out;
    out = nullptr;
}

void CNESCsvResult::handleFiles(const std::vector<SciFilePtr> & files)
{
}

void CNESCsvResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg)
{
    if (context.getSciFile().get() != current.get())
    {
        printRes();
        current = context.getSciFile();
    }
    if (const ast::FunctionDec * fd = context.topFn())
    {
        res[checker.getId(sub)].emplace_back(loc, msg, fd->getSymbol().getName());
    }
    else
    {
        res[checker.getId(sub)].emplace_back(loc, msg, L"");
    }
}

void CNESCsvResult::printRes()
{
    if (current.get())
    {
        for (const auto & r : res)
        {
            const std::string name = scilab::UTF8::toUTF8(r.first);
            std::string ruleName;
            auto i = tct.getRuleLink().find(name);
            if (i == tct.getRuleLink().end())
            {
                ruleName = getStr(r.first);
            }
            else
            {
                ruleName = getStr(scilab::UTF8::toWide(i->second.getStandardRuleId()));
            }
            for (const auto & info : r.second)
            {
                (*out) << ruleName << ','
                       << getStr(current->getFilename()) << ','
                       << getStr(info.funName) << ','
                       << getStr(info.msg) << ','
                       << info.loc.first_line << ','
                       << info.loc.first_column << '\n';
            }
        }
    }
    res.clear();
}

const std::string CNESCsvResult::getStr(const std::wstring & str)
{
    std::vector<wchar_t> buf;
    buf.reserve(str.size() + 2);
    buf.push_back(L'\"');
    for (auto c : str)
    {
        if (c == L'\"')
        {
            buf.push_back(L'\\');
            buf.push_back(L'\"');
        }
        else
        {
            buf.push_back(c);
        }
    }
    buf.push_back(L'\"');

    return scilab::UTF8::toUTF8(std::wstring(buf.begin(), buf.end()));
}

const std::string CNESCsvResult::getStr(const std::string & str)
{
    std::vector<char> buf;
    buf.reserve(str.size() + 2);
    buf.push_back('\"');
    for (auto c : str)
    {
        if (c == '\"')
        {
            buf.push_back('\\');
            buf.push_back('\"');
        }
        else
        {
            buf.push_back(c);
        }
    }
    buf.push_back('\"');

    return std::string(buf.begin(), buf.end());
}

} // namespace CNES

} // namespace slint
