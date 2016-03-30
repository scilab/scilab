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

#include "checkers/BreaksInLoopChecker.hxx"

namespace slint
{

void BreaksInLoopChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (maxBreaks >= 0 || maxContinues >= 0)
    {
        if (e.isForExp() || e.isWhileExp())
        {
            stack.emplace(&e, 0, 0);
        }
        else if (!stack.empty())
        {
            auto & tuple = stack.top();
            if (maxBreaks >= 0 && e.isBreakExp())
            {
                int & _breaks = std::get<1>(tuple);
                if (_breaks >= 0)
                {
                    if (_breaks == maxBreaks)
                    {
                        result.report(context, e.getLocation(), *this, 1, _("Number of break is limited: %d max."), maxBreaks);
                        _breaks = -1;
                    }
                    else
                    {
                        ++_breaks;
                    }
                }
            }
            else if (maxContinues >= 0 && e.isContinueExp())
            {
                int & _continues = std::get<2>(tuple);
                if (_continues >= 0)
                {
                    if (_continues == maxContinues)
                    {
                        result.report(context, e.getLocation(), *this, 2, _("Number of continue is limited: %d max."), maxContinues);
                        _continues = -1;
                    }
                    else
                    {
                        ++_continues;
                    }
                }
            }
        }
    }
}

void BreaksInLoopChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (maxBreaks >= 0 || maxContinues >= 0)
    {
        if (e.isForExp() || e.isWhileExp())
        {
            stack.pop();
        }
    }
}

const std::string BreaksInLoopChecker::getName() const
{
    return "BreaksInLoopChecker";
}

const std::wstring BreaksInLoopChecker::getId(const unsigned sub) const
{
    switch (sub)
    {
        case 0:
            return SLintChecker::getId();
        case 1:
            return SLintChecker::getId() + L".Break";
        case 2:
            return SLintChecker::getId() + L".Continue";
        default:
            return L"";
    }
}
}
