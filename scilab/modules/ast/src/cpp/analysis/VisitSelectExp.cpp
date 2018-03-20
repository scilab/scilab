/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#include <string>
#include <vector>

#include "AnalysisVisitor.hxx"
#include "generic_operations.hxx"
#include "tools.hxx"

namespace analysis
{

void AnalysisVisitor::visit(ast::SelectExp & e)
{
    logger.log(L"SelectExp", e.getLocation());
    e.getSelect()->accept(cv);
    ast::Exp & select = *e.getSelect();
    bool found = false;
    if (cv.getResult() && select.isConstExp())
    {
        ast::ConstExp & ce = static_cast<ast::ConstExp &>(select);
        types::InternalType * pIT = ce.getConstant();
        if (!pIT)
        {
            select.accept(cv.getExec());
            pIT = ce.getConstant();
        }
        if (pIT)
        {
            for (ast::exps_t::iterator i = std::next(e.getExps().begin()), end = e.hasDefault() ? std::prev(e.getExps().end()) : e.getExps().end(); i != end; ++i)
            {
                ast::CaseExp & casee = *static_cast<ast::CaseExp *>(*i);
                casee.getTest()->accept(cv);
                ast::Exp & test = *casee.getTest();
                if (cv.getResult() && test.isConstExp())
                {
                    ast::ConstExp & ce = static_cast<ast::ConstExp &>(test);
                    types::InternalType * pIT2 = ce.getConstant();
                    if (!pIT2)
                    {
                        test.accept(cv.getExec());
                        pIT2 = ce.getConstant();
                    }
                    if (pIT2)
                    {
                        types::InternalType * pIT3 = GenericComparisonEqual(pIT, pIT2);
                        if (pIT3)
                        {
                            if (pIT3->isTrue())
                            {
                                pIT3->killMe();
                                // We found the good case
                                e.replace(casee.getBody());
                                *i = nullptr;
                                casee.getBody()->accept(*this);
                                found = true;
                                break;
                            }
                            pIT3->killMe();
                        }
                    }
                }
            }
            if (!found && e.hasDefault())
            {
                e.getDefaultCase()->accept(*this);
                e.replace(e.getDefaultCase());
                *std::prev(e.getExps().end()) = nullptr;
                found = true;
            }
        }
    }

    if (!found)
    {
        dm.addBlock(Block::EXCLUSIVE, &e);
        e.getSelect()->accept(*this);
        ast::exps_t cases = e.getCases();
        for (auto exp : cases)
        {
            dm.addBlock(Block::NORMAL, exp);
            exp->accept(*this);
            dm.finalizeBlock();
        }

        if (e.getDefaultCase())
        {
            dm.addBlock(Block::NORMAL, e.getDefaultCase());
            e.getDefaultCase()->accept(*this);
            dm.finalizeBlock();
        }
        dm.finalizeBlock();
        analyze(e);
    }
}

void AnalysisVisitor::analyze(ast::SelectExp & e)
{
    enum Kind { INT, STRING, DUNNO };
    std::vector<int64_t> ints;
    std::vector<std::wstring> strs;
    ast::exps_t cases = e.getCases();
    Kind kind = DUNNO;
    bool first = true;

    for (ast::exps_t::iterator i = cases.begin(), end = cases.end(); i != end; ++i)
    {
        ast::CaseExp & casee = *static_cast<ast::CaseExp *>(*i);
        ast::Exp & test = *casee.getTest();
        if (test.isDoubleExp())
        {
            ast::DoubleExp & de = static_cast<ast::DoubleExp &>(test);
            if (first)
            {
                kind = INT;
                first = false;
            }
            else if (kind != INT)
            {
                kind = DUNNO;
            }

            int64_t ival;
            if (kind == INT && tools::asInteger<int64_t>(de.getValue(), ival))
            {
                ints.push_back(ival);
            }
        }
        else if (test.isStringExp())
        {
            ast::StringExp & se = static_cast<ast::StringExp &>(test);
            if (first)
            {
                kind = STRING;
                first = false;
            }
            else if (kind != STRING)
            {
                kind = DUNNO;
            }

            if (kind == STRING)
            {
                strs.push_back(se.getValue());
            }
        }
    }

    if (kind == INT)
    {
        const std::size_t size = ints.size();
        // Firstly, we check if we have a linear progression
        if (size > 2)
        {
            int64_t min, max;
            if (ints[0] < ints[1])
            {
                min = ints[0];
                max = ints[1];
            }
            else
            {
                min = ints[1];
                max = ints[0];
            }
            for (std::size_t i = 2; i < size; ++i)
            {
                const int64_t v = ints[i];
                if (v > max)
                {
                    max = v;
                }
                else if (v < min)
                {
                    min = v;
                }
            }

            const double diff = 1 - (double)size / (double)(max - min + 1);
            ast::exps_t * _cases = new ast::exps_t(cases);
            if (diff <= 0.25)
            {
                // we use a jump table

                ast::TableIntSelectExp * tise = nullptr;
                if (e.hasDefault())
                {
                    tise = new ast::TableIntSelectExp(e.getLocation(), *e.getSelect(), *_cases, *e.getDefaultCase(), ints, min, max);
                }
                else
                {
                    tise = new ast::TableIntSelectExp(e.getLocation(), *e.getSelect(), *_cases, ints, min, max);
                }
                tise->setLocation(e.getLocation());
                e.replace(tise);
            }
            else if (size >= 4)
            {
                // we use a map

                ast::MapIntSelectExp * mise = nullptr;
                if (e.hasDefault())
                {
                    mise = new ast::MapIntSelectExp(e.getLocation(), *e.getSelect(), *_cases, *e.getDefaultCase(), ints);
                }
                else
                {
                    mise = new ast::MapIntSelectExp(e.getLocation(), *e.getSelect(), *_cases, ints);
                }
                mise->setLocation(e.getLocation());
                e.replace(mise);
            }
            else
            {
                // we use a basic select

                ast::SmallIntSelectExp * sise = nullptr;
                if (e.hasDefault())
                {
                    sise = new ast::SmallIntSelectExp(e.getLocation(), *e.getSelect(), *_cases, *e.getDefaultCase(), ints);
                }
                else
                {
                    sise = new ast::SmallIntSelectExp(e.getLocation(), *e.getSelect(), *_cases, ints);
                }
                sise->setLocation(e.getLocation());
                e.replace(sise);
            }
        }
    }
    else if (kind == STRING)
    {
        ast::exps_t * _cases = new ast::exps_t(cases);
        ast::StringSelectExp * sse = nullptr;
        if (e.hasDefault())
        {
            sse = new ast::StringSelectExp(e.getLocation(), *e.getSelect(), *_cases, *e.getDefaultCase(), strs);
        }
        else
        {
            sse = new ast::StringSelectExp(e.getLocation(), *e.getSelect(), *_cases, strs);
        }
        sse->setLocation(e.getLocation());
        e.replace(sse);
    }
}
}
