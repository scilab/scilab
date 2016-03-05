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

#include "AnalysisVisitor.hxx"
#include "analyzers/IsrealAnalyzer.hxx"

namespace analysis
{
bool IsrealAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs != 1)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    const std::size_t size = args.size();
    if (size == 0 || size > 2)
    {
        return false;
    }

    ast::Exp * first = args.front();
    first->accept(visitor);
    Result & R1 = visitor.getResult();
    TIType & type1 = R1.getType();
    int ret = -1;

    if (type1.type == TIType::DOUBLE || type1.type == TIType::EMPTY)
    {
        ret = 1;
    }
    else if (type1.type == TIType::COMPLEX && size == 1)
    {
        ret = 0;
    }

    if (ret != -1)
    {
        TIType type(visitor.getGVN(), TIType::BOOLEAN);
        Result & res = e.getDecorator().setResult(type);
        res.getConstant() = new types::Bool(ret);
        e.getDecorator().setCall(L"isreal");
        visitor.setResult(res);

        return true;
    }

    return false;
}
}
