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

#include <fstream>
#include <iostream>

#include "parser.hxx"
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
#define DEFAULT_FILESPEC "*.*"
#else
#define DEFAULT_FILESPEC "*"
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
        scifile->getTree()->accept(visitor);
        visitor.postCheckFile();
    }
}

void SLint::setFiles(const std::string & files)
{
    std::vector<std::string> v = {files};
    setFiles(v);
}

void SLint::setFiles(types::String * files)
{
    const unsigned size = files->getSize();
    std::vector<std::string> filesVector;
    filesVector.reserve(size);

    for (unsigned i = 0; i < size; ++i)
    {
        filesVector.emplace_back(files->get(i));
    }
    setFiles(filesVector);
}

void SLint::setFiles(const std::vector<std::string> & files)
{
    for (const auto & file : files)
    {
        std::string full = getFullPath(file);
        if (!visitor.getOptions().isExcluded(full))
        {
            if (isdir(full.c_str()))
            {
                collectInDirectory(full);
            }
            else if (hasSuffix(full, ".sci"))
            {
                SciFilePtr sf = parseFile(full);
                if (sf.get())
                {
                    scifiles.emplace_back(sf);
                }
            }
        }
    }
}

const std::vector<SciFilePtr> & SLint::getFiles() const
{
    return scifiles;
}

void SLint::collectInDirectory(const std::string & path)
{
    std::string _path = path + DIR_SEPARATOR;
    int size = -1;

    char ** files = findfiles(_path.c_str(), DEFAULT_FILESPEC, &size, FALSE);
    if (size > 0 && files)
    {
        std::vector<std::string> filesVector;
        for (int i = 0; i < size; ++i)
        {
            filesVector.emplace_back(_path + files[i]);
        }
        freeArrayOfString(files, size);
        setFiles(filesVector);
    }
}

SciFilePtr SLint::parseFile(const std::string & filename)
{
    std::ifstream src(filename, std::ios::in | std::ios::binary | std::ios::ate);
    if (src.is_open())
    {
        src.seekg(0, src.end);
        int len = static_cast<int>(src.tellg());
        src.seekg (0, src.beg);
        char * buffer = new char[len + 1];
        buffer[len] = '\0';
        src.read(buffer, len);
        src.close();

        Parser parser;
        ThreadManagement::LockParser();
        parser.parse(buffer);

        if (parser.getExitStatus() != Parser::Succeded)
        {
            FREE(buffer);
            delete parser.getTree();
            ThreadManagement::UnlockParser();
            throw FileException(filename, parser.getErrorMessage());
            //return SciFilePtr(nullptr);
        }
        else
        {
            ThreadManagement::UnlockParser();
            return SciFilePtr(new SciFile(filename, buffer, parser.getTree()));
        }
    }

    throw FileException(filename, _("Cannot open the file"));
}

bool SLint::hasSuffix(const std::string & filename, const std::string & suffix)
{
    return filename.size() >= suffix.size() && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string SLint::getFullPath(const std::string & path)
{
    const unsigned SIZE = PATH_MAX * 4;
    char * fullpath = (char *)MALLOC(SIZE * sizeof(char));
    char * expandedPath = expandPathVariable(path.c_str());
    std::string _fullpath;
    if (get_full_path(fullpath, expandedPath, SIZE))
    {
        FREE(expandedPath);
        _fullpath = std::string(fullpath);
    }
    else
    {
        _fullpath = std::string(expandedPath);
        FREE(expandedPath);
    }
    FREE(fullpath);

    return _fullpath;
}
} // namespace slint
