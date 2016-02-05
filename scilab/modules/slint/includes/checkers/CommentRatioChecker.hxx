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

#ifndef __SLINT_COMMENT_RATIO_CHECKER_HXX__
#define __SLINT_COMMENT_RATIO_CHECKER_HXX__

#include <cstdint>

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Compute the ratio between commented lines and total lines
 */
class CommentRatioChecker : public SLintChecker
{

    uint64_t count;
    const double ratioMin;

public:

    CommentRatioChecker(const std::wstring & checkerId, const double _ratioMin) : SLintChecker(checkerId), count(0), ratioMin(_ratioMin) { }
    ~CommentRatioChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;
    bool isFileChecker() const;
    void preCheckFile(SLintContext & context, SLintResult & result);
    void postCheckFile(SLintContext & context, SLintResult & result);

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::COMMENTEXP };
    }
};

} // namespace slint

#endif // __SLINT_COMMENT_RATIO_CHECKER_HXX__
