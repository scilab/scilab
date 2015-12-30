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

#include "AnalysisVisitor.hxx"
#include "analyzers/TypeAnalyzer.hxx"
#include "tools.hxx"
#include "double.hxx"

namespace analysis
{
bool TypeAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
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

    ast::Exp * arg = args.back();
    arg->accept(visitor);
    const double code = visitor.getResult().getType().getScilabCode();
    if (code != -1)
    {
        TIType type(visitor.getGVN(), TIType::DOUBLE);
        Result & res = e.getDecorator().setResult(type);
        res.getConstant() = visitor.getGVN().getValue(code);
        e.getDecorator().setCall("type");
        visitor.setResult(res);
        return true;
    }
    else
    {
        return false;
    }
}
}
