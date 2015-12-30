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
#include "analyzers/InttypeAnalyzer.hxx"
#include "tools.hxx"
#include "double.hxx"

namespace analysis
{
bool InttypeAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
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
    TIType & typ = visitor.getResult().getType();
    double code;
    switch (typ.type)
    {
        case TIType::EMPTY :
        case TIType::DOUBLE :
            code = 0;
            break;
        case TIType::INT8 :
            code = 1;
            break;
        case TIType::INT16 :
            code = 2;
            break;
        case TIType::INT32 :
            code = 4;
            break;
        case TIType::INT64 :
            code = 8;
            break;
        case TIType::UINT8 :
            code = 11;
            break;
        case TIType::UINT16 :
            code = 12;
            break;
        case TIType::UINT32 :
            code = 14;
            break;
        case TIType::UINT64 :
            code = 18;
            break;
        default :
            code = -1;
    }

    if (code != -1)
    {
        TIType type(visitor.getGVN(), TIType::DOUBLE);
        Result & res = e.getDecorator().setResult(type);
        res.getConstant() = visitor.getGVN().getValue(code);
        e.getDecorator().setCall("inttype");
        visitor.setResult(res);
        return true;
    }
    else
    {
        return false;
    }
}
}
