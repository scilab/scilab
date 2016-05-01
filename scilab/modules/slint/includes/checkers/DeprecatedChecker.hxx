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

#ifndef __SLINT_DEPRECATED_CHECKER_HXX__
#define __SLINT_DEPRECATED_CHECKER_HXX__

#include <memory>
#include <unordered_map>

#include "SLintChecker.hxx"

namespace slint
{

/**
 * Check if a function is deprecated.
 */
class DeprecatedChecker : public SLintChecker
{

    class __Svd : public SLintChecker
    {
        void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
        void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) { }
        const std::string getName() const
        {
            return "svd";
        }

        virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
        {
            return {  };
        }
    };

    class __Mfprintf : public SLintChecker
    {
        void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
        void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) { }
        const std::string getName() const
        {
            return "mfprintf";
        }

        virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
        {
            return {  };
        }
    };

    static std::unordered_map<std::wstring, std::wstring> deprecated;
    static std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> partiallyDeprecated;

public:

    DeprecatedChecker(const std::wstring & checkerId) : SLintChecker(checkerId) { }
    ~DeprecatedChecker() { }

    void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result);
    const std::string getName() const;

    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const
    {
        return { ast::Exp::CALLEXP };
    }

private:

    static std::unordered_map<std::wstring, std::wstring> initDep();
    static std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> initPartDep();
};

} // namespace slint

#endif // __SLINT_DEPRECATED_CHECKER_HXX__
