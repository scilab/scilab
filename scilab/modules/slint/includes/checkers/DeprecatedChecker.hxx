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
