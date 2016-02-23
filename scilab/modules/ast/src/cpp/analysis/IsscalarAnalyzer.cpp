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
#include "analyzers/IsscalarAnalyzer.hxx"

namespace analysis
{
bool IsscalarAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs != 1)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    if (args.size() != 1)
    {
        return false;
    }

    ast::Exp * first = args.front();
    first->accept(visitor);
    Result & R1 = visitor.getResult();
    TIType & type1 = R1.getType();

    if (!type1.ismatrix())
    {
        return false;
    }

    if (type1.isscalar())
    {
        TIType type(visitor.getGVN(), TIType::BOOLEAN);
        Result & res = e.getDecorator().setResult(type);
        res.getConstant() = new types::Bool(1);
        e.getDecorator().setCall(L"isscalar");
        visitor.setResult(res);
        return true;
    }

    return false;
}
}
