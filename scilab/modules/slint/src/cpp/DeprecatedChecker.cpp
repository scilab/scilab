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

#include "checkers/DeprecatedChecker.hxx"

namespace slint
{

std::unordered_map<std::wstring, std::wstring> DeprecatedChecker::deprecated = initDep();
std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> DeprecatedChecker::partiallyDeprecated = initPartDep();

void DeprecatedChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    if (ce.getName().isSimpleVar())
    {
        const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
        const auto i = deprecated.find(name);
        if (i != deprecated.end())
        {
            if (i->second.empty())
            {
                result.report(context, e.getLocation(), *this, _("Deprecated function: %s."), name);
            }
            else
            {
                result.report(context, e.getLocation(), *this, _("Deprecated function %s: use %s instead."), name, i->second);
            }
        }
        else
        {
            const auto i = partiallyDeprecated.find(name);
            if (i != partiallyDeprecated.end())
            {
                i->second->preCheckNode(e, context, result);
            }
        }
    }
}

void DeprecatedChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string DeprecatedChecker::getName() const
{
    return "DeprecatedChecker";
}

void DeprecatedChecker::__Svd::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    const ast::exps_t args = ce.getArgs();
    if (args.size() == 2)
    {
        const ast::Exp & second = *args.back();
        if (second.isDoubleExp() && static_cast<const ast::DoubleExp &>(second).getValue() == 0)
        {
            result.report(context, e.getLocation(), *this, _("svd(..., 0) is deprecated."));
        }
    }
}

void DeprecatedChecker::__Mfprintf::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    const ast::exps_t args = ce.getArgs();
    if (args.size() != 0)
    {
        const ast::Exp & first = *args.front();
        if (first.isDoubleExp() && static_cast<const ast::DoubleExp &>(first).getValue() == -1)
        {
            result.report(context, e.getLocation(), *this, _("mfprintf(-1, ...) is deprecated."));
        }
    }
}

std::unordered_map<std::wstring, std::wstring> DeprecatedChecker::initDep()
{
    // TODO: get this list from a conf file
    std::unordered_map<std::wstring, std::wstring> map;
    map.emplace(L"znaupd", L"eigs");
    map.emplace(L"zneupd", L"eigs");
    map.emplace(L"dseupd", L"eigs");
    map.emplace(L"dneupd", L"eigs");
    map.emplace(L"dnaupd", L"eigs");
    map.emplace(L"dsaupd", L"eigs");
    map.emplace(L"m_circle", L"");
    map.emplace(L"lex_sort", L"gsort");
    map.emplace(L"plot2d1", L"plot2d");
    map.emplace(L"plotframe", L"");
    map.emplace(L"xset", L"set");
    map.emplace(L"xget", L"get");
    map.emplace(L"xclear", L"clf");
    map.emplace(L"xpause", L"sleep");
    map.emplace(L"havewindow", L"getscilabmode");
    map.emplace(L"datatipSetStruct", L"");
    map.emplace(L"datatipGetStruct", L"");
    map.emplace(L"fcontour2d", L"contour2d");
    map.emplace(L"fcontour", L"contour");
    map.emplace(L"fac3d", L"plot3d");
    map.emplace(L"eval3d", L"ndgrid");
    map.emplace(L"str2code", L"");
    map.emplace(L"code2str", L"");
    map.emplace(L"perl", L"");
    map.emplace(L"gspec", L"spec");
    map.emplace(L"gschur", L"schur");
    map.emplace(L"rafiter", L"taucs_chsolve");
    map.emplace(L"numdiff", L"numderivative");
    map.emplace(L"derivative", L"numderivative");
    map.emplace(L"mvvacov", L"cov");
    map.emplace(L"strcmpi", L"strcmp");
    map.emplace(L"jconvMatrixMethod", L"jautoTranspose");
    return map;
}

std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> DeprecatedChecker::initPartDep()
{
    std::unordered_map<std::wstring, std::shared_ptr<SLintChecker>> map;
    map.emplace(L"svd", std::shared_ptr<SLintChecker>(new __Svd()));
    map.emplace(L"mfprintf", std::shared_ptr<SLintChecker>(new __Mfprintf()));

    return map;
}
}
