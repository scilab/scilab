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

#include "checkers/SemicolonAtEOLChecker.hxx"

namespace slint
{
    void SemicolonAtEOLChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
    {
        if (e.isSeqExp())
        {
            const ast::exps_t & exps = static_cast<const ast::SeqExp &>(e).getExps();
            if (!exps.empty())
            {
                int prevline = exps.front()->getLocation().last_line;
                ast::Exp * prevexp = exps.front();
                for (ast::exps_t::const_iterator i = std::next(exps.begin()), end = exps.end(); i != end; ++i)
                {
                    if ((*i)->getLocation().first_line != prevline)
                    {
                        check(prevexp, context, result);
                    }
                    prevexp = *i;
                    prevline = prevexp->getLocation().last_line;
                }
                check(prevexp, context, result);
            }
        }
    }

    void SemicolonAtEOLChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
    {
    }

    const std::string SemicolonAtEOLChecker::getName() const
    {
        return "SemicolonAtEOLChecker";
    }

    void SemicolonAtEOLChecker::check(const ast::Exp * e, SLintContext & context, SLintResult & result) const
    {
        ast::Exp::ExpType ty = e->getType();
        if (ty != ast::Exp::IFEXP && ty != ast::Exp::WHILEEXP && ty != ast::Exp::TRYCATCHEXP && ty != ast::Exp::FOREXP && ty != ast::Exp::FUNCTIONDEC && ty != ast::Exp::COMMENTEXP && ty != ast::Exp::SELECTEXP && ty != ast::Exp::SEQEXP)
        {
            std::pair<unsigned int, unsigned int> out;
            if (context.getPosition(e->getLocation(), out) && out.first < out.second)
            {
                const wchar_t * line = context.getCode() + out.first;
                const unsigned len = out.second - out.first;
                for (unsigned i = len - 1; i >= 0; --i)
                {
                    const wchar_t c = line[i];
                    if (c == L' ' || c == L'\t')
                    {
                        continue;
                    }
                    else if (c == L';')
                    {
                        break;
                    }
                    else
                    {
                        result.report(context, e->getLocation(), *this, _("Instruction not finished by a semi-colon."));
                        break;
                    }
                }
            }
        }
    }
}
