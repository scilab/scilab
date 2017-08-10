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

    std::stack<std::unordered_map<std::wstring, std::tuple<Location, bool, ast::AssignListExp *>>> assigned;
    std::stack<std::unordered_map<std::wstring, const ast::Exp *>> used;

public:

    VariablesChecker(const std::wstring & checkerId) : SLintChecker(checkerId) { }
    ~VariablesChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;
    const std::wstring getId(const unsigned sub) const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::FUNCTIONDEC, ast::Exp::SIMPLEVAR, ast::Exp::VARDEC, ast::Exp::WHILEEXP, ast::Exp::FOREXP };
    }

private:

    static bool isParentOf(const ast::Exp * potentialParent, const ast::Exp * potentialChild);
};

} // namespace slint

#endif // __SLINT_VARIABLES_CHECKER_HXX__
