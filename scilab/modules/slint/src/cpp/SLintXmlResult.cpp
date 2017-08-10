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

SLintXmlResult::SLintXmlResult(const std::wstring & _path) : current(nullptr), path(_path)
{
    const std::wstring fullpath = SLint::getFullPath(path);
    out = new std::ofstream(scilab::UTF8::toUTF8(fullpath), std::ios::out);
    if (!out->is_open())
    {
        delete out;
        out = nullptr;
        throw FileException(fullpath, L"Can\'t open it.");
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
    if (current.get())
    {
        (*out) << "  </File>\n";
    }

    (*out) << "</SLintResult>\n";
    out->close();
    delete out;
    out = nullptr;
}

const std::string SLintXmlResult::getStr(const std::wstring & str)
{
    return scilab::UTF8::toUTF8(replaceByEntities(str));
}

void SLintXmlResult::handleMessage(SLintContext & context, const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg)
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
    print(loc, checker, sub, msg);
}

void SLintXmlResult::print(const SciFilePtr & file)
{
    (*out) << "  <File name=\"" << getStr(file->getFilename()) << "\">\n";
}

void SLintXmlResult::print(const Location & loc, const SLintChecker & checker, const unsigned sub, const std::wstring & msg)
{
    (*out) << "    <Result>\n";
    print(loc);
    print(checker, sub);
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

void SLintXmlResult::print(const SLintChecker & checker, const unsigned sub)
{
    (*out) << "      <Checker name=\"" << checker.getName()
        << "\" id=\"" << getStr(checker.getId(sub))
        << "\"/>\n";
}

void SLintXmlResult::print(const std::wstring & msg)
{
    (*out) << "      <Message text=\"" << getStr(msg)
        << "\"/>\n";
}

std::wstring SLintXmlResult::replaceByEntities(const std::wstring & seq)
{
    std::vector<wchar_t> buf;
    for (auto c : seq)
    {
        if (c == L'<')
        {
            pushEntity(buf, L"&lt;", 4);
        }
        else if (c == L'>')
        {
            pushEntity(buf, L"&gt;", 4);
        }
        else if (c == L'\'')
        {
            pushEntity(buf, L"&apos;", 6);
        }
        else if (c == L'\"')
        {
            pushEntity(buf, L"&quot;", 6);
        }
        else if (c == L'&')
        {
            pushEntity(buf, L"&amp;", 5);
        }
        else
        {
            buf.push_back(c);
        }
    }

    return std::wstring(buf.begin(), buf.end());
}

} // namespace slint
