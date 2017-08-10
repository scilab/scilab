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

#include "checkers/SelectChecker.hxx"

namespace slint
{

void SelectChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::SelectExp & se = static_cast<const ast::SelectExp &>(e);
    if (checkDefault && !se.hasDefault())
    {
        result.report(context, e.getLocation(), *this, _("Select should have a default case."));
    }
    const ast::exps_t cases = se.getCases();
    if (cases.size() == 0)
    {
        if (checkEmpty)
        {
            result.report(context, e.getLocation(), *this, _("Select with no case."));
        }
    }
    else if (cases.size() == 1)
    {
        if (checkOneCase)
        {
            result.report(context, e.getLocation(), *this, _("Select with one case: it could be replaced by an if statement."));
        }
    }
    else if (checkHomogeneity)
    {
        enum Type { STRING, NUMBER, OTHER };
        const ast::Exp & first = *static_cast<const ast::CaseExp *>(cases.front())->getTest();
        Type type;
        if (first.isStringExp())
        {
            type = STRING;
        }
        else if (first.isDoubleExp())
        {
            type = NUMBER;
        }
        else
        {
            type = OTHER;
            result.report(context, first.getLocation(), *this, _("Case expression should be a number or string."));
        }

        if (type != OTHER)
        {
            for (ast::exps_t::const_iterator i = std::next(cases.begin()), end = cases.end(); i != end; ++i)
            {
                const ast::Exp & test = *static_cast<const ast::CaseExp *>(*i)->getTest();
                if (type == STRING)
                {
                    if (!test.isStringExp())
                    {
                        result.report(context, test.getLocation(), *this, _("Case expression types are mixed."));
                        break;
                    }
                }
                else if (type == NUMBER)
                {
                    if (!test.isDoubleExp())
                    {
                        result.report(context, test.getLocation(), *this, _("Case expression types are mixed."));
                        break;
                    }
                }
            }
        }
    }
}

void SelectChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string SelectChecker::getName() const
{
    return "SelectChecker";
}
}
