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

#include "checkers/DeprecatedChecker.hxx"

namespace slint
{

std::unordered_map<std::string, std::string> DeprecatedChecker::deprecated = initDep();
std::unordered_map<std::string, std::shared_ptr<SLintChecker>> DeprecatedChecker::partiallyDeprecated = initPartDep();

void DeprecatedChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    if (ce.getName().isSimpleVar())
    {
        const std::string & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
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

std::unordered_map<std::string, std::string> DeprecatedChecker::initDep()
{
    // TODO: get this list from a conf file
    std::unordered_map<std::string, std::string> map;
    map.emplace("znaupd", "eigs");
    map.emplace("zneupd", "eigs");
    map.emplace("dseupd", "eigs");
    map.emplace("dneupd", "eigs");
    map.emplace("dnaupd", "eigs");
    map.emplace("dsaupd", "eigs");
    map.emplace("m_circle", "");
    map.emplace("lex_sort", "gsort");
    map.emplace("plot2d1", "plot2d");
    map.emplace("plotframe", "");
    map.emplace("xset", "set");
    map.emplace("xget", "get");
    map.emplace("isoview", "");
    map.emplace("xclear", "");
    map.emplace("datatipSetStruct", "");
    map.emplace("datatipGetStruct", "");
    map.emplace("fcontour2d", "contour2d");
    map.emplace("fcontour", "contour");
    map.emplace("fac3d", "plot3d");
    map.emplace("str2code", "");
    map.emplace("code2str", "");
    map.emplace("perl", "");
    map.emplace("gspec", "spec");
    map.emplace("gschur", "schur");
    map.emplace("rafiter", "taucs_chsolve");
    map.emplace("numdiff", "numderivative");
    map.emplace("derivative", "numderivative");
    map.emplace("mvvacov", "cov");

    return map;
}

std::unordered_map<std::string, std::shared_ptr<SLintChecker>> DeprecatedChecker::initPartDep()
{
    std::unordered_map<std::string, std::shared_ptr<SLintChecker>> map;
    map.emplace("svd", std::shared_ptr<SLintChecker>(new __Svd()));
    map.emplace("mfprintf", std::shared_ptr<SLintChecker>(new __Mfprintf()));

    return map;
}
}
