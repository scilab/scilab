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

#ifndef __SLINT_FUNCTION_NAME_CHECKER_HXX__
#define __SLINT_FUNCTION_NAME_CHECKER_HXX__

#include "SLintChecker.hxx"
#include "PCREMatcher.hxx"

namespace slint
{

/**
 * Check the function name against a PCRE pattern
 */
class FunctionNameChecker : public SLintChecker
{

    PCREMatcher matcher;
    int minimalLen;
    int maximalLen;

public:

    FunctionNameChecker(const std::wstring & checkerId, const std::wstring & pattern, int _minimalLen, int _maximalLen) : SLintChecker(checkerId), matcher(pattern), minimalLen(_minimalLen), maximalLen(_maximalLen) { }
    ~FunctionNameChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::FUNCTIONDEC };
    }
};

} // namespace slint

#endif // __SLINT_FUNCTION_NAME_CHECKER_HXX__
