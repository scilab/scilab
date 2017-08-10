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

#include "checkers/SLintChecker.hxx"

namespace slint
{
std::unordered_set<std::wstring> SLintChecker::constants = SLintChecker::init();

std::unordered_set<std::wstring> SLintChecker::init()
{
    std::unordered_set<std::wstring> _constants;
    _constants.emplace(L"%pi");
    _constants.emplace(L"%eps");
    _constants.emplace(L"%e");
    _constants.emplace(L"%i");
    _constants.emplace(L"%nan");
    _constants.emplace(L"%inf");
    _constants.emplace(L"%t");
    _constants.emplace(L"%f");
    _constants.emplace(L"%T");
    _constants.emplace(L"%F");
    _constants.emplace(L"SCI");
    _constants.emplace(L"WSCI");
    _constants.emplace(L"SCIHOME");
    _constants.emplace(L"TMPDIR");

    return _constants;
}

bool SLintChecker::isScilabConstant(const std::wstring & name)
{
    return constants.find(name) != constants.end();
}

void SLintChecker::setData(void * _data)
{
    data = _data;
}

void * SLintChecker::getData() const
{
    return data;
}

bool SLintChecker::isFileChecker() const
{
    return false;
}

void SLintChecker::preCheckFile(SLintContext & context, SLintResult & result)
{

}

void SLintChecker::postCheckFile(SLintContext & context, SLintResult & result)
{

}

const std::wstring & SLintChecker::getId() const
{
    return checkerId;
}

const std::wstring SLintChecker::getId(const unsigned sub) const
{
    return sub ? L"" : getId();
}
    
}
