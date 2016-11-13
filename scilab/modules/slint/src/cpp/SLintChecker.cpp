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

const std::string SLintChecker::getId(const unsigned sub) const
{
    return sub ? "" : getId();
}
    
}
