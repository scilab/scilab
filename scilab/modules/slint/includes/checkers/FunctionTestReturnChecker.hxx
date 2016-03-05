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

#ifndef __SLINT_FUNCTION_TEST_RETURN_CHECKER_HXX__
#define __SLINT_FUNCTION_TEST_RETURN_CHECKER_HXX__

#include <unordered_map>
#include <vector>

#include "SLintChecker.hxx"
#include "FindSymVisitor.hxx"

namespace slint
{

/**
 * Test error indicator returned by function:
 * [fd, err] = mopen(...)
 * if err == ... then
 */
class FunctionTestReturnChecker : public SLintChecker
{

    std::unordered_map<std::wstring, std::vector<unsigned int>> funs;

public:

    FunctionTestReturnChecker(const std::wstring & checkerId, const std::unordered_map<std::wstring, std::vector<unsigned int>> & _funs) : SLintChecker(checkerId), funs(_funs) { }
    ~FunctionTestReturnChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::CALLEXP };
    }
};

} // namespace slint

#endif // __SLINT_FUNCTION_TEST_RETURN_CHECKER_HXX__
