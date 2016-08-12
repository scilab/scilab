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

#include "SciFile.hxx"

extern "C"
{
#include "sci_malloc.h"
}

namespace slint
{

SciFile::SciFile() : code(nullptr), tree(nullptr), main(nullptr), codeLength(0)
{
    initLines();
}

SciFile::SciFile(const std::wstring & _filename, const wchar_t * _code, const ast::Exp * _tree) : filename(_filename), code(_code), tree(_tree), main(nullptr)
{
    initLines();
    analyzeTree();
}

SciFile::~SciFile()
{
    FREE(const_cast<wchar_t *>(code));
    delete tree;
}

const std::wstring & SciFile::getFilename() const
{
    return filename;
}

const wchar_t * SciFile::getCode() const
{
    return code;
}

unsigned int SciFile::getCodeLength() const
{
    return codeLength;
}

const ast::Exp * SciFile::getTree() const
{
    return tree;
}

bool SciFile::getPosition(const Location & loc, std::pair<unsigned int, unsigned int> & out) const
{
    if (loc.first_line > lines.size() || loc.last_line > lines.size())
    {
        return false;
    }
    out.first = lines[loc.first_line - 1].first + loc.first_column - 1;
    out.second = lines[loc.last_line - 1].first + loc.last_column - 1;

    return out.first <= codeLength && out.second <= codeLength;
}

bool SciFile::getFromPositionToEOL(const Location & loc, std::pair<unsigned int, unsigned int> & out) const
{
    if (loc.first_line + 1 > lines.size())
    {
        return false;
    }
    out.first = lines[loc.first_line - 1].first + loc.first_column - 1;
    out.second = lines[loc.last_line - 1].second + 1;

    return out.first <= codeLength;
}

bool SciFile::checkLineLength(const unsigned int max, std::vector<unsigned int> & out) const
{
    unsigned int i = 1;
    for (const auto & line : lines)
    {
        if (line.second - line.first + 1 > max)
        {
            out.push_back(i);
        }
        ++i;
    }
    return out.empty();
}

unsigned int SciFile::countLines() const
{
    unsigned int i = 0;
    for (const auto & line : lines)
    {
        if (line.first < line.second && !isEmptyLine(code + line.first, line.second - line.first + 1))
        {
            ++i;
        }
    }
    return i;
}

unsigned int SciFile::countLines(const unsigned from, const unsigned to) const
{
    unsigned int i = 0;
    for (unsigned j = from; j <= to; ++j)
    {
        const auto & line = lines[j - 1];
        if (line.first < line.second && !isEmptyLine(code + line.first, line.second - line.first + 1))
        {
            ++i;
        }
    }
    return i;
}

void SciFile::initLines()
{
    lines.clear();
    codeLength = 0;
    if (code)
    {
        const wchar_t * p = code;
        lines.emplace_back(0, 0);
        while (*p)
        {
            if (*p == L'\n') // Unix EOL
            {
                lines.back().second = p - code - 1;
                lines.emplace_back(p - code + 1, 0);
                ++p;
            }
            else if (*p == L'\r')
            {
                if (*(p + 1) == L'\n') // Windows EOL
                {
                    lines.back().second = p - code - 1;
                    lines.emplace_back(p - code + 2, 0);
                    p += 2;
                }
                else // Old mac EOL
                {
                    lines.back().second = p - code - 1;
                    lines.emplace_back(p - code + 1, 0);
                    ++p;
                }
            }
            else
            {
                ++p;
            }
        }

        lines.back().second = p - code - 1;
        codeLength = p - code;
    }

    /*for (const auto & line : lines)
      {
      std::wcerr << line.first << "->" << line.second << std::endl;
      }*/
}

bool SciFile::isEmptyLine(const wchar_t * line, const unsigned len) const
{
    // An empty line is (^[\t ]*//) or (^[\t ]*$)
    for (unsigned i = 0; i < len; ++i)
    {
        const wchar_t c = line[i];
        if (c == L' ' || c == L'\t')
        {
            continue;
        }
        else if (c == L'/' && (i < len - 1) && line[i + 1] == L'/')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void SciFile::analyzeTree()
{
    if (tree && tree->isSeqExp())
    {
        bool first = true;
        const ast::SeqExp * se = static_cast<const ast::SeqExp *>(tree);
        for (const auto e : se->getExps())
        {
            if (e->isFunctionDec())
            {
                const ast::FunctionDec * fd = static_cast<const ast::FunctionDec *>(e);
                if (first)
                {
                    main = fd;
                    first = false;
                }
                else
                {
                    privateFunctions.emplace(fd->getSymbol().getName(), fd);
                }
            }
        }
    }
}

bool SciFile::isPrivateFunction(const symbol::Symbol & sym) const
{
    return privateFunctions.find(sym.getName()) != privateFunctions.end();
}

const ast::FunctionDec * SciFile::getPrivateFunction(const std::wstring & name) const
{
    auto i = privateFunctions.find(name);
    if (i != privateFunctions.end())
    {
        return i->second;
    }
    return nullptr;
}

const ast::FunctionDec * SciFile::getMain() const
{
    return main;
}

} // namespace slint
