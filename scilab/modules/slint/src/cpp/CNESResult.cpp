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

#include <chrono>
#include <ctime>
#include <string>

#include "output/SLintXmlResult.hxx"
#include "output/cnes/CNESResult.hxx"
#include "checkers/SLintChecker.hxx"
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

CNESResult::CNESResult(types::String * conf, const std::wstring & id, const std::wstring & _path) : current(nullptr), path(_path)
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

CNESResult::~CNESResult()
{
    if (out)
    {
        out->close();
        delete out;
    }
}

void CNESResult::finalize()
{
    printRes();
    (*out) << "</analysisProject>\n";
    out->close();
    delete out;
    out = nullptr;
}

void CNESResult::handleFiles(const std::vector<SciFilePtr> & files)
{
    for (const auto & file : files)
    {
        (*out) << "  <analysisFile filename=\"" << SLintXmlResult::getStr(file->getFilename()) << "\""
               << " language=\"scilab\" />\n";
    }
}

void CNESResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::wstring & msg)
{
    if (context.getSciFile().get() != current.get())
    {
        printRes();
        current = context.getSciFile();
    }
    res[checker.getId()].emplace_back(loc, msg);
}

void CNESResult::printRes()
{
    if (current.get())
    {
        for (const auto & r : res)
        {
            (*out) << "  <analysisRule analysisRuleId=\"" << SLintXmlResult::getStr(r.first) << "\">\n";
            for (const auto & p : r.second)
            {
                (*out) << "    <result filename=\"" << SLintXmlResult::getStr(current->getFilename()) << "\""
                       << " resultMessage=\"" << SLintXmlResult::getStr(p.second) << "\""
                       << " resultLine=\"" << p.first.first_line << "\""
                       << " resultColumn=\"" << p.first.first_column << "\" />\n";
            }
            (*out) << "  </analysisRule>\n";
        }
    }
    res.clear();
}

} // namespace CNES

} // namespace slint
