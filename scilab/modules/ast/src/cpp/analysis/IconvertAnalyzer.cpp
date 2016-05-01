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
#include "analyzers/IconvertAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{
bool IconvertAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs != 1)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    if (args.size() != 2)
    {
        return false;
    }

    ast::Exp * first = args.front();
    ast::Exp * second = args.back();

    first->accept(visitor);
    Result R1 = visitor.getResult();
    TIType & type1 = R1.getType();
    if (!type1.ismatrix())
    {
        return false;
    }
    second->accept(visitor);
    Result & R2 = visitor.getResult();

    double val;
    unsigned char ival;

    if (R2.getConstant().getDblValue(val) && tools::asInteger<unsigned char>(val, ival))
    {
        TIType::Type type;

        switch (ival)
        {
            case 0:
                type = TIType::DOUBLE;
                break;
            case 1:
                type = TIType::INT8;
                break;
            case 2:
                type = TIType::INT16;
                break;
            case 4:
                type = TIType::INT32;
                break;
            case 8:
                type = TIType::INT64;
                break;
            case 11:
                type = TIType::UINT8;
                break;
            case 12:
                type = TIType::UINT16;
                break;
            case 14:
                type = TIType::UINT32;
                break;
            case 18:
                type = TIType::UINT64;
                break;
            default:
                return false;
        }

        TIType typ(visitor.getGVN(), type, type1.rows, type1.cols);
        Result & res = e.getDecorator().setResult(typ);
        e.getDecorator().setCall(L"iconvert");
        visitor.setResult(res);
        return true;
    }

    return false;
}
}
