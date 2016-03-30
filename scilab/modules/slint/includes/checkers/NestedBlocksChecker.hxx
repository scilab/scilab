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

#ifndef __SLINT_NESTED_BLOCKS_CHECKER_HXX__
#define __SLINT_NESTED_BLOCKS_CHECKER_HXX__

#include <vector>

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check the number of nested blocks
 */
class NestedBlocksChecker : public SLintChecker
{

    std::vector<const ast::Exp *> stack;
    const int max;

public:

    NestedBlocksChecker(const std::wstring & checkerId, const int _max) : SLintChecker(checkerId), max(_max) { }
    ~NestedBlocksChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::IFEXP, ast::Exp::WHILEEXP, ast::Exp::FOREXP, ast::Exp::SELECTEXP };
    }
};

} // namespace slint

#endif // __SLINT_NESTED_BLOCKS_CHECKER_HXX__
