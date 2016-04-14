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

#include "output/SLintXmlResult.hxx"
#include "output/cnes/CNESXmlResult.hxx"
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

CNESXmlResult::CNESXmlResult(const ToolConfiguration & tc, types::String * conf, const std::wstring & id, const std::wstring & _path) : tct(tc.getToolConfiguration()), current(nullptr), path(_path)
{
    std::string projectName;
    std::string projectVersion;
    std::string projectAuthor;
    if (conf)
    {
        if (conf->getSize() >= 4)
        {
            projectName = SLintXmlResult::getStr(conf->get(3));
        }
        if (conf->getSize() >= 5)
        {
            projectVersion = SLintXmlResult::getStr(conf->get(4));
        }
        if (conf->getSize() >= 6)
        {
            projectAuthor = SLintXmlResult::getStr(conf->get(5));
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
        (*out) << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
               << "<analysisProject";
        if (!projectName.empty())
        {
            (*out) << " analysisProjectName=\"" << projectName << "\"";
        }
        if (!projectVersion.empty())
        {
            (*out) << " analysisProjectVersion=\"" << projectVersion << "\"";
        }
        (*out) << ">\n"
               << "  <analysisInformations analysisConfigurationId=\"" << SLintXmlResult::getStr(id) << "\"";
        if (!projectAuthor.empty())
        {
            (*out) << " author=\"" << projectAuthor << "\"";
        }

        std::chrono::time_point<std::chrono::system_clock> point = std::chrono::system_clock::now();;
        std::time_t time = std::chrono::system_clock::to_time_t(point);
        std::string date(std::ctime(&time));
        if (date.back() == '\n')
        {
            date = std::string(date.begin(), std::prev(date.end()));
        }
        (*out) << " analysisDate=\"" << date << "\" />\n";
    }
}

CNESXmlResult::~CNESXmlResult()
{
    if (out)
    {
        out->close();
        delete out;
    }
}

void CNESXmlResult::finalize()
{
    printRes();
    (*out) << "</analysisProject>\n";
    out->close();
    delete out;
    out = nullptr;
}

void CNESXmlResult::handleFiles(const std::vector<SciFilePtr> & files)
{
    for (const auto & file : files)
    {
        (*out) << "  <analysisFile filename=\"" << SLintXmlResult::getStr(file->getFilename()) << "\""
               << " language=\"scilab\" />\n";
    }
}

void CNESXmlResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg)
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

void CNESXmlResult::printRes()
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
                ruleName = SLintXmlResult::getStr(r.first);
            }
            else
            {
                ruleName = SLintXmlResult::getStr(scilab::UTF8::toWide(i->second.getStandardRuleId()));
            }
            (*out) << "  <analysisRule analysisRuleId=\"" << ruleName << "\">\n";
            for (const auto & info : r.second)
            {
                (*out) << "    <result fileName=\"" << SLintXmlResult::getStr(current->getFilename()) << "\""
                       << " resultNamePlace=\"" << SLintXmlResult::getStr(info.funName) << "\""
                       << " resultMessage=\"" << SLintXmlResult::getStr(info.msg) << "\""
                       << " resultLine=\"" << info.loc.first_line << "\""
                       << " resultColumn=\"" << info.loc.first_column << "\" />\n";
            }
            (*out) << "  </analysisRule>\n";
        }
    }
    res.clear();
}

} // namespace CNES

} // namespace slint
