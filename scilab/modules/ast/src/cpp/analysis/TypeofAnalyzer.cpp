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
#include "analyzers/TypeofAnalyzer.hxx"
#include "tools.hxx"
#include "string.hxx"

namespace analysis
{
bool TypeofAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
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
    const std::string & str = visitor.getResult().getType().getScilabString();

    if (!str.empty())
    {
        TIType type(visitor.getGVN(), TIType::STRING);
        Result & res = e.getDecorator().setResult(type);
        res.getConstant() = new types::String(str.c_str());
        e.getDecorator().setCall("typeof");
        visitor.setResult(res);
        return true;
    }
    else
    {
        visitor.getDM().releaseTmp(visitor.getResult().getTempId(), arg);
        return false;
    }
}
}
