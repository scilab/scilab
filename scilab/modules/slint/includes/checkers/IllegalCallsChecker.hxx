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

#ifndef __SLINT_ILLEGAL_CALLS_CHECKER_HXX__
#define __SLINT_ILLEGAL_CALLS_CHECKER_HXX__

#include <set>

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check for functions whicn are not allowed
 */
class IllegalCallsChecker : public SLintChecker
{
    std::set<std::wstring> illegal;

public:

    IllegalCallsChecker(const std::wstring & checkerId, const std::vector<std::wstring> & _illegal) : SLintChecker(checkerId), illegal(_illegal.begin(), _illegal.end()) { }
    ~IllegalCallsChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::SIMPLEVAR };
    }
};

} // namespace slint

#endif // __SLINT_ILLEGAL_CALLS_CHECKER_HXX__
