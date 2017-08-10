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

#ifndef __SLINT_BREAKS_IN_LOOP_CHECKER_HXX__
#define __SLINT_BREAKS_IN_LOOP_CHECKER_HXX__

#include <stack>
#include <tuple>

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Count the breaks or continues in a loop
 */
class BreaksInLoopChecker : public SLintChecker
{

    std::stack<std::tuple<const ast::Exp *, int, int>> stack;
    const int maxBreaks;
    const int maxContinues;

public:

    BreaksInLoopChecker(const std::wstring & checkerId, const int _maxBreaks, const int _maxContinues) : SLintChecker(checkerId), maxBreaks(_maxBreaks), maxContinues(_maxContinues) { }
    ~BreaksInLoopChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;
    const std::wstring getId(const unsigned sub) const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::FOREXP, ast::Exp::WHILEEXP, ast::Exp::BREAKEXP, ast::Exp::CONTINUEEXP };
    }

};

} // namespace slint

#endif // __SLINT_BREAKS_IN_LOOP_CHECKER_HXX__
