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

#ifndef __SLINT_SEMICOLON_AT_EOL_CHECKER_HXX__
#define __SLINT_SEMICOLON_AT_EOL_CHECKER_HXX__

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check if there is a semicolon at the end of line
 */
class SemicolonAtEOLChecker : public SLintChecker
{

public:

    SemicolonAtEOLChecker(const std::wstring & checkerId) : SLintChecker(checkerId) { }
    ~SemicolonAtEOLChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::FUNCTIONDEC };
    }

private:

    void check(const ast::Exp * e, SLintContext & context, SLintResult & result) const;
    void checkSeqExp(const ast::SeqExp & e, SLintContext & context, SLintResult & result) const;
};

} // namespace slint

#endif // __SLINT_SEMICOLON_AT_EOL_CHECKER_HXX__
