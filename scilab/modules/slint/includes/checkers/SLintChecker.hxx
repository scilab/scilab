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

#ifndef ___SLINT_CHECKER_HXX__
#define ___SLINT_CHECKER_HXX__

#include <string>
#include <unordered_set>
#include <vector>

#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"

#include "SLintContext.hxx"
#include "output/SLintResult.hxx"

extern "C"
{
#include "localization.h"
}

namespace slint
{

class SLintChecker
{

    void * data;
    const std::wstring checkerId;

protected:

    static std::unordered_set<std::wstring> constants;

public:

    SLintChecker(const std::wstring & _checkerId) : data(nullptr), checkerId(_checkerId) { }
    SLintChecker() : data(nullptr), checkerId(L"") { }

    virtual ~SLintChecker() { }

    /**
     * Check a node before to visit it
     */
    virtual void preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) = 0;

    /**
     * Check a node after visited it
     */
    virtual void postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result) = 0;

    /**
     * Get the checker name
     */
    virtual const std::string getName() const = 0;

    /**
     * Get the nodes in AST where this checker is plugged
     */
    virtual const std::vector<ast::Exp::ExpType> getAstNodes() const = 0;

    /**
     * Check if this checker must be applied on a file
     */
    virtual bool isFileChecker() const;

    /**
     * Check the file before to visit it
     */
    virtual void preCheckFile(SLintContext & context, SLintResult & result);

    /**
     * Check the file after visited it
     */
    virtual void postCheckFile(SLintContext & context, SLintResult & result);

    /**
     * Set opaque data
     */
    virtual void setData(void * _data);

    /**
     * Get opaque data
     */
    virtual void * getData() const;

    /**
     * Get the checker id
     */
    virtual const std::wstring & getId() const;

    /**
     * Get the checker id for the sub rule
     */
    virtual const std::wstring getId(const unsigned sub) const;

    /**
     * Check if the name corresponds to a Scilab's constant
     */
    static bool isScilabConstant(const std::wstring & name);

private:

    static std::unordered_set<std::wstring> init();
};

} // namespace slint

#endif // __SLINT_CHECKER_HXX__
