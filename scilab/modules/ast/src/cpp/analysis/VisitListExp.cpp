/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AnalysisVisitor.hxx"
#include "tools.hxx"

namespace analysis
{
void AnalysisVisitor::visit(ast::ListExp & e)
{
    double start, step, end;
    if (AnalysisVisitor::asDouble(e.getStart(), start) && AnalysisVisitor::asDouble(e.getStep(), step) && AnalysisVisitor::asDouble(e.getEnd(), end))
    {
        double out;
        int type = ForList64::checkList(start, end, step, out);

        switch (type)
        {
            case 0:
                e.getDecorator().res = Result(TIType(dm.getGVN(), TIType::EMPTY), -1);
                break;
            case 1:
                e.getDecorator().res = Result(TIType(dm.getGVN(), TIType::DOUBLE), -1);
                break;
            case 2:
            {
                const uint64_t N = ForList64::size(start, end, step);
                TIType T(dm.getGVN(), TIType::DOUBLE, 1, N);
                if (N == 1)
                {
                    out = start;
                }
                e.getDecorator().res = Result(T, temp.add(T));
                break;
            }
            default:
                break;
        }
        e.setValues(start, step, end, out);
        setResult(e.getDecorator().res);

        return;
    }

    e.getStart().accept(*this);
    Result Rstart = getResult();

    if (!AnalysisVisitor::asDouble(e.getStep(), step) || (step != -1 && step != 1))
    {
        e.getDecorator().res = Result(TIType(dm.getGVN(), Rstart.getType().type, false, true), -1);
        setResult(e.getDecorator().res);

        return;
    }

    GVN::Value * gvnStart;

    if (!Rstart.getType().isscalar())
    {
        e.getDecorator().res = Result(TIType(dm.getGVN(), Rstart.getType().type, false, true), -1);
        setResult(e.getDecorator().res);

        return;
    }

    if (Rstart.getValue(start))
    {
        if (Rstart.getIntType() == tools::NOTANINT)
        {
            gvnStart = getGVN().getValue((double)tools::cast<int>(start + step));
        }
        else
        {
            gvnStart = getGVN().getValue((double)tools::cast<int>(start));
        }
    }
    else
    {
        gvnStart = Rstart.getGVNValue();
        if (!gvnStart)
        {
            e.getDecorator().res = Result(TIType(dm.getGVN(), Rstart.getType().type, false, true), -1);
            setResult(e.getDecorator().res);

            return;
        }
    }

    e.getEnd().accept(*this);
    Result & Rend = getResult();
    GVN::Value * gvnEnd;

    if (Rend.getValue(end))
    {
        if (Rend.getIntType() == tools::NOTANINT)
        {
            gvnEnd = getGVN().getValue((double)tools::cast<int>(end - step));
        }
        else
        {
            gvnEnd = getGVN().getValue((double)tools::cast<int>(end));
        }
    }
    else
    {
        gvnEnd = Rend.getGVNValue();
        if (!gvnEnd)
        {
            e.getDecorator().res = Result(TIType(dm.getGVN(), Rstart.getType().type, false, true), -1);
            setResult(e.getDecorator().res);

            return;
        }
    }

    GVN::Value * ONEValue = getGVN().getValue(1.);
    SymbolicDimension ONE(getGVN(), ONEValue);
    GVN::Value * v;
    if (step == 1)
    {
        v = getGVN().getValue(OpValue::Kind::MINUS, *gvnEnd, *gvnStart);
    }
    else
    {
        v = getGVN().getValue(OpValue::Kind::MINUS, *gvnStart, *gvnEnd);
    }
    v = getGVN().getValue(OpValue::Kind::PLUS, *v, *ONEValue);

    if (v->poly->isConstant() && v->poly->constant <= 0)
    {
        TIType type(getGVN(), TIType::EMPTY);
        e.getDecorator().res = Result(type);
    }
    else
    {
        bool res = getCM().check(ConstraintManager::POSITIVE, v);
        if (res)
        {
            TIType type(getGVN(), TIType::DOUBLE, ONE, SymbolicDimension(getGVN(), v));
            e.getDecorator().res = Result(type);
        }
        else
        {
            e.getDecorator().res = Result(TIType(dm.getGVN(), Rstart.getType().type, false, true), -1);
            setResult(e.getDecorator().res);

            return;
        }
    }
    setResult(e.getDecorator().res);
}
}
