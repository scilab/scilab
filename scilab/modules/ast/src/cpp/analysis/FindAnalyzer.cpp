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

#include "AnalysisVisitor.hxx"
#include "analyzers/FindAnalyzer.hxx"
#include "tools.hxx"
#include "double.hxx"

namespace analysis
{
bool FindAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs > 2)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    if (args.size() != 1 && args.size() != 2)
    {
        return false;
    }

    ast::Exp * first = args[0];
    first->accept(visitor);
    Result & res = visitor.getResult();
    TIType & ty = res.getType();
    if (ty.ismatrix())
    {
        TIType type(visitor.getGVN(), TIType::DOUBLE, false);
        Result & _res = e.getDecorator().setResult(type);
        if (lhs == 1)
        {
            _res.setMaxIndex(ty.rows * ty.cols);
            visitor.setResult(_res);

            return true;
        }
        else if (lhs == 2)
        {
            std::vector<Result> & mlhs = visitor.getLHSContainer();
            mlhs.clear();
            mlhs.reserve(2);
            mlhs.emplace_back(type);
            mlhs.back().setMaxIndex(ty.rows);
            mlhs.emplace_back(type);
            mlhs.back().setMaxIndex(ty.cols);

            return true;
        }
    }

    return false;
}
}
