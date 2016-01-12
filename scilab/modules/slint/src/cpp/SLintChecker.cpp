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

#include "checkers/SLintChecker.hxx"

namespace slint
{
std::unordered_set<std::string> SLintChecker::constants = SLintChecker::init();

std::unordered_set<std::string> SLintChecker::init()
{
    std::unordered_set<std::string> _constants;
    _constants.emplace("%pi");
    _constants.emplace("%eps");
    _constants.emplace("%e");
    _constants.emplace("%i");
    _constants.emplace("%nan");
    _constants.emplace("%inf");
    _constants.emplace("%t");
    _constants.emplace("%f");
    _constants.emplace("%T");
    _constants.emplace("%F");
    _constants.emplace("SCI");
    _constants.emplace("WSCI");
    _constants.emplace("SCIHOME");
    _constants.emplace("TMPDIR");

    return _constants;
}

bool SLintChecker::isScilabConstant(const std::string & name)
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

const std::string & SLintChecker::getId() const
{
    return checkerId;
}
}
