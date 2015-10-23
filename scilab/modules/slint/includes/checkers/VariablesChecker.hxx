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

#ifndef __SLINT_VARIABLES_CHECKER_HXX__
#define __SLINT_VARIABLES_CHECKER_HXX__

#include <stack>
#include <string>
#include <unordered_map>

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check for unused or not defined variables
 */
class VariablesChecker : public SLintChecker
{

    std::stack<std::unordered_map<std::wstring, std::pair<Location, ast::AssignListExp *>>> assigned;
    std::stack<std::unordered_map<std::wstring, const ast::Exp *>> used;

public:

    VariablesChecker(const std::wstring & checkerId) : SLintChecker(checkerId) { }
    ~VariablesChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::FUNCTIONDEC, ast::Exp::SIMPLEVAR, ast::Exp::VARDEC, ast::Exp::WHILEEXP, ast::Exp::FOREXP };
    }

private:

    static bool isParentOf(const ast::Exp * potentialParent, const ast::Exp * potentialChild);
};

} // namespace slint

#endif // __SLINT_VARIABLES_CHECKER_HXX__
