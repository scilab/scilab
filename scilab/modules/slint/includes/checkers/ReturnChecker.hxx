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

#ifndef __SLINT_RETURN_CHECKER_HXX__
#define __SLINT_RETURN_CHECKER_HXX__

#include <stack>

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check for the number of returns in a function
 */
class ReturnChecker : public SLintChecker
{

    std::stack<std::pair<const ast::Exp *, int>> stack;
    const int maxReturns;

public:

    ReturnChecker(const std::wstring & checkerId, const int _maxReturns) : SLintChecker(checkerId), maxReturns(_maxReturns) { }
    ~ReturnChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::FUNCTIONDEC, ast::Exp::RETURNEXP };
    }

};

} // namespace slint

#endif // __SLINT_RETURN_CHECKER_HXX__
