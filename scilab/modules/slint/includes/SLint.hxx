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

#ifndef __SLINT_HXX__
#define __SLINT_HXX__

#include <memory>
#include <string>
#include <vector>

#include "output/SLintResult.hxx"
#include "SLintContext.hxx"
#include "SLintOptions.hxx"
#include "SLintVisitor.hxx"
#include "FileException.hxx"
#include "SciFile.hxx"
#include "string.hxx"

namespace slint
{

class SLint
{
    SLintContext context;
    SLintVisitor visitor;

    std::vector<SciFilePtr> scifiles;

public:

    SLint(SLintOptions & options, SLintResult & result);

    void setFiles(const std::vector<std::wstring> & files);
    void setFiles(types::String * files);
    void setFiles(const std::wstring & files);
    const std::vector<SciFilePtr> & getFiles() const;
    void check();
    const SLintVisitor & getVisitor() const;

    static std::wstring getFullPath(const std::wstring & path);

private:

    SciFilePtr parseFile(const std::wstring & filename);
    void collectInDirectory(const std::wstring & path);

    static bool hasSuffix(const std::wstring & filename, const std::wstring & suffix);
};

} // namespace slint

#endif // __SLINT_HXX__
