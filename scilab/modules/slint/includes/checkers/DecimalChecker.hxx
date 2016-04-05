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

    const std::wstring character;
    const bool checkDot;

public:

    DecimalChecker(const std::wstring & checkerId, const std::wstring & _character) : SLintChecker(checkerId), character(_character), checkDot(false) { }
    DecimalChecker(const std::wstring & checkerId, const bool _checkDot) : SLintChecker(checkerId), character(L'\0'), checkDot(_checkDot) { }
    DecimalChecker(const std::wstring & checkerId, const std::wstring & _character, const bool _checkDot) : SLintChecker(checkerId), character(_character), checkDot(_checkDot) { }

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
