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
    logger.log(L"ListExp", e.getLocation());
    e.getStart().accept(*this);
    Result & Rstart = e.getStart().getDecorator().getResult();
    e.getEnd().accept(*this);
    Result & Rend = e.getEnd().getDecorator().getResult();
    e.getStep().accept(*this);
    Result & Rstep = e.getStep().getDecorator().getResult();

    if (e.getParent()->isVarDec())
    {
        ast::VarDec & vd = *static_cast<ast::VarDec *>(e.getParent());
        const symbol::Symbol & sym = vd.getSymbol();
        GVN::Value * startRange = nullptr;
        GVN::Value * endRange = nullptr;
        if (Rstart.getConstant().getGVNValue(getGVN(), startRange) && Rend.getConstant().getGVNValue(getGVN(), endRange))
        {
            TIType typ(dm.getGVN(), TIType::DOUBLE);
            Result & res = e.getDecorator().setResult(Result(typ, -1));
            res.setRange(SymbolicRange(getGVN(), startRange, endRange));
            setResult(res);
            return;
        }
        SymbolicRange & rangeStart = Rstart.getRange();
        if (rangeStart.isValid())
        {
            if (endRange || Rend.getConstant().getGVNValue(getGVN(), endRange))
            {
                // start is an iterator and end is not
                TIType typ(dm.getGVN(), TIType::DOUBLE);
                Result & res = e.getDecorator().setResult(Result(typ, -1));
                res.setRange(SymbolicRange(getGVN(), rangeStart.getStart(), endRange));
                setResult(res);
                return;
            }
            else
            {
                SymbolicRange & rangeEnd = Rend.getRange();
                if (rangeEnd.isValid())
                {
                    TIType typ(dm.getGVN(), TIType::DOUBLE);
                    Result & res = e.getDecorator().setResult(Result(typ, -1));
                    res.setRange(SymbolicRange(getGVN(), rangeStart.getStart(), rangeEnd.getEnd()));
                    setResult(res);
                    return;
                }
            }
        }
        // TODO: finish all the cases
    }

    double start = 1;
    double step = 1;
    double end = 1;
    if (Rstart.getConstant().getDblValue(start) && Rstep.getConstant().getDblValue(step) && Rend.getConstant().getDblValue(end))
    {
        double out;
        int type = ForList64::checkList(start, end, step, out);

        switch (type)
        {
            case 0:
                e.getDecorator().setResult(Result(TIType(dm.getGVN(), TIType::EMPTY), -1));
                break;
            case 1:
                e.getDecorator().setResult(Result(TIType(dm.getGVN(), TIType::DOUBLE), -1));
                break;
            case 2:
            {
                const uint64_t N = ForList64::size(start, end, step);
                if (e.getParent()->isVarDec())
                {
                    TIType T(dm.getGVN(), TIType::DOUBLE, 1, 1);
                    e.getDecorator().setResult(Result(T));
                }
                else
                {
                    TIType T(dm.getGVN(), TIType::DOUBLE, 1, N);
                    if (N == 1)
                    {
                        out = start;
                    }
                    e.getDecorator().setResult(Result(T, dm.getTmpId(T, false)));
                }
                break;
            }
            default:
                break;
        }
        e.setValues(start, step, end, out);
        setResult(e.getDecorator().res);

        return;
    }

    if (step == 0 || tools::isNaN(step) || !tools::isFinite(step)
            || tools::isNaN(start) || !tools::isFinite(start)
            ||  tools::isNaN(end) || !tools::isFinite(end))
    {
        e.getDecorator().setResult(Result(TIType(dm.getGVN(), TIType::EMPTY), -1));
        return;
    }

    if (!Rstep.getConstant().getDblValue(step) || (step != -1 && step != 1))
    {
        Result & res = e.getDecorator().setResult(Result(TIType(dm.getGVN(), Rstart.getType().type, false), -1));
        setResult(res);
        return;
    }

    if (!Rstart.getType().isscalar() || !Rend.getType().isscalar())
    {
        Result & res = e.getDecorator().setResult(Result(TIType(dm.getGVN(), Rstart.getType().type, false), -1));
        setResult(res);
        return;
    }

    GVN::Value * gvnStart;
    if (Rstart.getConstant().getDblValue(start))
    {
        if (tools::getIntType(start) == tools::NOTANINT)
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
        gvnStart = Rstart.getConstant().getGVNValue();
        if (!gvnStart)
        {
            Result & res = e.getDecorator().setResult(Result(TIType(dm.getGVN(), Rstart.getType().type, false), -1));
            setResult(res);
            return;
        }
    }

    GVN::Value * gvnEnd;

    if (Rend.getConstant().getDblValue(end))
    {
        if (tools::getIntType(end) == tools::NOTANINT)
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
        gvnEnd = Rend.getConstant().getGVNValue();
        if (!gvnEnd)
        {
            Result & res = e.getDecorator().setResult(Result(TIType(dm.getGVN(), Rstart.getType().type, false), -1));
            setResult(res);
            return;
        }
    }

    GVN::Value * ONEValue = getGVN().getValue(1);
    SymbolicDimension ONE(getGVN(), ONEValue);
    GVN::Value * v;

    if (gvnStart->value == gvnEnd->value)
    {
        Result & res = e.getDecorator().setResult(Result(TIType(getGVN(), TIType::DOUBLE, ONE, ONE)));
        setResult(res);
        return;
    }

    if (step == 1)
    {
        v = getGVN().getValue(OpValue::Kind::MINUS, *gvnEnd, *gvnStart);
    }
    else
    {
        v = getGVN().getValue(OpValue::Kind::MINUS, *gvnStart, *gvnEnd);
    }
    v = getGVN().getValue(OpValue::Kind::PLUS, *v, *ONEValue);

    if (v->poly->constant < 0 && v->poly->isCoeffNegative(false))
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
            e.getDecorator().setResult(type);
        }
        else
        {
            Result & res = e.getDecorator().setResult(Result(TIType(dm.getGVN(), Rstart.getType().type, false), -1));
            setResult(res);
            return;
        }
    }

    setResult(e.getDecorator().res);
}
}
