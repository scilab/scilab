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
        e.getDecorator().setCall(L"inttype");
        visitor.setResult(res);
        return true;
    }
    else
    {
        return false;
    }
}
}
