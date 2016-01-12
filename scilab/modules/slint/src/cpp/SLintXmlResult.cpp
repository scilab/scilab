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

#include <string>

#include "output/SLintXmlResult.hxx"
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

SLintXmlResult::SLintXmlResult(const std::string & _path) : current(nullptr), path(_path)
{
    const std::string fullpath = SLint::getFullPath(path);
    out = new std::ofstream(fullpath, std::ios::out);
    if (!out->is_open())
    {
        delete out;
        out = nullptr;
        throw FileException(fullpath, "Can\'t open it.");
    }
    else
    {
        (*out) << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
               << "<SLintResult>\n";
    }
}

SLintXmlResult::~SLintXmlResult()
{
    if (out)
    {
        out->close();
        delete out;
    }
}

void SLintXmlResult::finalize()
{
    (*out) << "</SLintResult>\n";
    out->close();
    delete out;
    out = nullptr;
}

const std::string SLintXmlResult::getStr(const std::string & str)
{
    return replaceByEntities(str);
}

void SLintXmlResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const std::string & msg)
{
    if (context.getSciFile().get() != current.get())
    {
        if (current.get())
        {
            (*out) << "  </File>\n";
        }
        current = context.getSciFile();
        print(current);
    }
    print(loc, checker, msg);
}

void SLintXmlResult::print(const SciFilePtr & file)
{
    (*out) << "  <File name=\"" << getStr(file->getFilename()) << "\">\n";
}

void SLintXmlResult::print(const Location & loc, const SLintChecker & checker, const std::string & msg)
{
    (*out) << "    <Result>\n";
    print(loc);
    print(checker);
    print(msg);
    (*out) << "    </Result>\n";
}

void SLintXmlResult::print(const Location & loc)
{
    (*out) << "      <Location first_line=\"" << loc.first_line
           << "\" first_column=\"" << loc.first_column
           << "\" last_line=\"" << loc.last_line
           << "\" last_column=\"" << loc.last_column
           << "\"/>\n";
}

void SLintXmlResult::print(const SLintChecker & checker)
{
    (*out) << "      <Checker name=\"" << checker.getName()
           << "\" id=\"" << getStr(checker.getId())
           << "\"/>\n";
}

void SLintXmlResult::print(const std::string & msg)
{
    (*out) << "      <Message text=\"" << getStr(msg)
           << "\"/>\n";
}

std::string SLintXmlResult::replaceByEntities(const std::string & seq)
{
    std::vector<char> buf;
    for (auto c : seq)
    {
        if (c == '<')
        {
            pushEntity(buf, "&#0060;", 7);
        }
        else if (c == '>')
        {
            pushEntity(buf, "&#0062;", 7);
        }
        else if (c == '\'')
        {
            pushEntity(buf, "&#0039;", 7);
        }
        else if (c == '\"')
        {
            pushEntity(buf, "&#0034;", 7);
        }
        else if (c == '&')
        {
            pushEntity(buf, "&#0038;", 7);
        }
        else
        {
            buf.push_back(c);
        }
    }

    return std::string(buf.begin(), buf.end());
}

} // namespace slint
