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

#ifndef __SLINT_TODO_CHECKER_HXX__
#define __SLINT_TODO_CHECKER_HXX__

#include "SLintChecker.hxx"
#include "PCREMatcher.hxx"

namespace slint
{

/**
 * Check if the keyword TODO is the comments
 */
class TodoChecker : public SLintChecker
{

    PCREMatcher todo;

public:

    TodoChecker(const std::wstring & checkerId) : SLintChecker(checkerId), todo(L"[tT][oO][dD][oO][^a-zA-Z0-9]+") { }
    ~TodoChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::COMMENTEXP };
    }
};

} // namespace slint

#endif // __SLINT_TODO_CHECKER_HXX__
