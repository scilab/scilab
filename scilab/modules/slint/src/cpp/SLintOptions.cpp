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
