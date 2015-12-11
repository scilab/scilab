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
