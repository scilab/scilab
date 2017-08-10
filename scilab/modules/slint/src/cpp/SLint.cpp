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

#include <fstream>
#include <iostream>

#include "parser.hxx"
#include "prettyprintvisitor.hxx"
#include "SLint.hxx"
#include "threadmanagement.hxx"
#include "UTF8.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "os_string.h"
#include "expandPathVariable.h"
#include "localization.h"
#include "isdir.h"
#include "findfiles.h"
#include "freeArrayOfString.h"
#include "fullpath.h"
#include "PATH_MAX.h"
}

#ifdef _MSC_VER
#define DEFAULT_FILESPEC L"*.*"
#else
#define DEFAULT_FILESPEC L"*"
#endif

//#define SLINT_PRINT_AST
#ifdef SLINT_PRINT_AST
#include "prettyprintvisitor.hxx"
#endif

namespace slint
{

SLint::SLint(SLintOptions & options, SLintResult & result) : context(*this), visitor(context, result)
{
    visitor.setOptions(options);
}

const SLintVisitor & SLint::getVisitor() const
{
    return visitor;
}

void SLint::check()
{
    visitor.getResult().handleFiles(scifiles);
    for (const auto & scifile : scifiles)
    {
        context.setSciFile(scifile);
        visitor.preCheckFile();

#ifdef SLINT_PRINT_AST
        ast::PrettyPrintVisitor ppv(std::wcerr, false, true);
        scifile->getTree()->accept(ppv);
#undef SLINT_PRINT_AST
#endif

        scifile->getTree()->accept(visitor);
        visitor.postCheckFile();
    }
}

void SLint::setFiles(const std::wstring & files)
{
    std::vector<std::wstring> v = {files};
    setFiles(v);
}

void SLint::setFiles(types::String * files)
{
    const unsigned size = files->getSize();
    std::vector<std::wstring> filesVector;
    filesVector.reserve(size);

    for (unsigned i = 0; i < size; ++i)
    {
        filesVector.emplace_back(files->get(i));
    }
    setFiles(filesVector);
}

void SLint::setFiles(const std::vector<std::wstring> & files)
{
    for (const auto & file : files)
    {
        std::wstring full = getFullPath(file);
        if (!visitor.getOptions().isExcluded(full))
        {
            if (isdirW(full.c_str()))
            {
                collectInDirectory(full);
            }
            else if (hasSuffix(full, L".sci"))
            {
                SciFilePtr sf = parseFile(full);
                if (sf.get())
                {
                    scifiles.emplace_back(sf);
                    context.addPublicFunction(sf->getMain());
                }
            }
        }
    }
}

const std::vector<SciFilePtr> & SLint::getFiles() const
{
    return scifiles;
}

void SLint::collectInDirectory(const std::wstring & path)
{
    std::wstring _path = path + DIR_SEPARATORW;
    int size = -1;

    wchar_t ** files = findfilesW(_path.c_str(), DEFAULT_FILESPEC, &size, FALSE);
    if (size > 0 && files)
    {
        std::vector<std::wstring> filesVector;
        for (int i = 0; i < size; ++i)
        {
            filesVector.emplace_back(_path + files[i]);
        }
        freeArrayOfWideString(files, size);
        setFiles(filesVector);
    }
}

SciFilePtr SLint::parseFile(const std::wstring & filename)
{
    std::ifstream src(scilab::UTF8::toUTF8(filename), std::ios::in | std::ios::binary | std::ios::ate);
    if (src.is_open())
    {
        src.seekg(0, src.end);
        int len = static_cast<int>(src.tellg());
        src.seekg (0, src.beg);
        char * buffer = new char[len + 1];
        buffer[len] = '\0';
        src.read(buffer, len);
        src.close();

        wchar_t * _buffer = to_wide_string(buffer);
        delete[] buffer;
        Parser parser;

        ThreadManagement::LockParser();
        parser.parse(_buffer);

        if (parser.getExitStatus() != Parser::Succeded)
        {
            FREE(_buffer);
            delete parser.getTree();
            ThreadManagement::UnlockParser();
            throw FileException(filename, parser.getErrorMessage());
            //return SciFilePtr(nullptr);
        }
        else
        {
            ThreadManagement::UnlockParser();
            return SciFilePtr(new SciFile(filename, _buffer, parser.getTree()));
        }
    }

    wchar_t * error = to_wide_string(_("Cannot open the file"));
    std::wstring _error(error);
    FREE(error);

    throw FileException(filename, _error);
}

bool SLint::hasSuffix(const std::wstring & filename, const std::wstring & suffix)
{
    return filename.size() >= suffix.size() && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::wstring SLint::getFullPath(const std::wstring & path)
{
    const unsigned SIZE = PATH_MAX * 4;
    wchar_t * fullpath = (wchar_t *)MALLOC(SIZE * sizeof(wchar_t));
    wchar_t * expandedPath = expandPathVariableW(const_cast<wchar_t *>(path.c_str()));
    std::wstring _fullpath;
    if (get_full_pathW(fullpath, expandedPath, SIZE))
    {
        FREE(expandedPath);
        _fullpath = std::wstring(fullpath);
    }
    else
    {
        _fullpath = std::wstring(expandedPath);
        FREE(expandedPath);
    }
    FREE(fullpath);

    return _fullpath;
}
} // namespace slint
