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

#ifndef __EXP_IN_COND_CHECKER_HXX__
#define __EXP_IN_COND_CHECKER_HXX__

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check the number of expements in a condition (if or while)
 */
class ExpInCondChecker : public SLintChecker
{

    const unsigned int max;

public:

    ExpInCondChecker(const std::wstring & checkerId, const int _max) : SLintChecker(checkerId), max(_max) { }
    ~ExpInCondChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::WHILEEXP, ast::Exp::IFEXP };
    }

private:

    unsigned int checkCond(const ast::Exp & e) const;

};

} // namespace slint

#endif // __EXP_IN_COND_CHECKER_HXX__
