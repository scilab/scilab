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

#ifndef __SLINT_DECIMAL_CHECKER_HXX__
#define __SLINT_DECIMAL_CHECKER_HXX__

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check if there is a number before a dot and if the exponent in scientific notation is the correct letter:
 * - 0.1234 instead of .1234
 * - 1e10 instead of 1D10
 */
class DecimalChecker : public SLintChecker
{

    const wchar_t character;
    const bool checkDot;

public:

    DecimalChecker(const std::wstring & checkerId, const wchar_t _character) : SLintChecker(checkerId), character(_character), checkDot(false) { }
    DecimalChecker(const std::wstring & checkerId, const bool _checkDot) : SLintChecker(checkerId), character(L'\0'), checkDot(_checkDot) { }
    DecimalChecker(const std::wstring & checkerId, const wchar_t _character, const bool _checkDot) : SLintChecker(checkerId), character(_character), checkDot(_checkDot) { }

    ~DecimalChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::DOUBLEEXP };
    }

};

} // namespace slint

#endif // __SLINT_DECIMAL_CHECKER_HXX__
