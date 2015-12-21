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

#include "SLintOptions.hxx"
#include "UTF8.hxx"

namespace slint
{

SLintOptions::SLintOptions() { }

void SLintOptions::addDefault(SLintChecker * checker)
{
    std::shared_ptr<SLintChecker> _checker(checker);
    const std::vector<ast::Exp::ExpType> nodeTypes = checker->getAstNodes();
    for (const auto type : nodeTypes)
    {
        checkers.emplace(type, _checker);
    }
    if (checker->isFileChecker())
    {
        fileCheckers.emplace_back(_checker);
    }
}

SLintOptions::MapCheckers & SLintOptions::getCheckers()
{
    return checkers;
}

SLintOptions::FileCheckers & SLintOptions::getFileCheckers()
{
    return fileCheckers;
}

void SLintOptions::addExcludedFile(const std::string & filename)
{
    excludedFiles.emplace(scilab::UTF8::toWide(filename));
}

bool SLintOptions::isExcluded(const std::wstring & filename) const
{
    return excludedFiles.find(filename) != excludedFiles.end();
}

void SLintOptions::setId(const std::string & _id)
{
    id = scilab::UTF8::toWide(_id);
}

const std::wstring & SLintOptions::getId() const
{
    return id;
}

} // namespace slint
