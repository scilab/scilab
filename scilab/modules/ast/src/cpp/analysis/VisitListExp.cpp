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
#include "tools.hxx"

namespace analysis
{

void AnalysisVisitor::visit(ast::ListExp & e)
{
    logger.log(L"ListExp", e.getLocation());
    if (e.getParent()->isVarDec())
    {
        visitInVarDecCtxt(e);
        return;
    }

    e.getStart().accept(*this);
    Result & Rstart = e.getStart().getDecorator().getResult();
    e.getEnd().accept(*this);
    Result & Rend = e.getEnd().getDecorator().getResult();
    e.getStep().accept(*this);
    Result & Rstep = e.getStep().getDecorator().getResult();

    double start = 1;
    double step = 1;
    double end = 1;
    if (Rstart.getConstant().getDblValue(start) && Rstep.getConstant().getDblValue(step) && Rend.getConstant().getDblValue(end))
    {
        // Start, Step & End are constant !
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
                TIType T(dm.getGVN(), TIType::DOUBLE, 1, N);
                if (N == 1)
                {
                    out = start;
                }
                e.getDecorator().setResult(Result(T, dm.getTmpId(T, false)));
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

    GVN::Value * ONEValue = getGVN().getValue(int64_t(1));
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

void AnalysisVisitor::visitInVarDecCtxt(ast::ListExp & e)
{
    e.getStart().accept(*this);
    Result & Rstart = e.getStart().getDecorator().getResult();
    e.getEnd().accept(*this);
    Result & Rend = e.getEnd().getDecorator().getResult();
    e.getStep().accept(*this);
    Result & Rstep = e.getStep().getDecorator().getResult();

    double start = 1;
    double step = 1;
    double end = 1;
    if ((Rstart.getConstant().getDblValue(start) || Rstep.getConstant().getDblValue(step)
            || Rend.getConstant().getDblValue(end)) &&
            (step == 0 || tools::isNaN(step) || !tools::isFinite(step)
             || tools::isNaN(start) || !tools::isFinite(start)
             ||  tools::isNaN(end) || !tools::isFinite(end)))
    {
        // We have an invalid list
        e.getDecorator().setResult(Result(TIType(dm.getGVN(), TIType::EMPTY), -1));
        return;
    }

    ast::VarDec & vd = *static_cast<ast::VarDec *>(e.getParent());
    const symbol::Symbol & sym = vd.getSymbol();
    GVN::Value * startRange = nullptr;
    GVN::Value * endRange = nullptr;
    Result & res = e.getDecorator().setResult(Result(TIType(dm.getGVN(), TIType::DOUBLE), -1));

    if (Rstart.getConstant().getGVNValue(getGVN(), startRange) && Rend.getConstant().getGVNValue(getGVN(), endRange))
    {
        // Start & End are GVN values
        res.setRange(SymbolicRange(getGVN(), startRange, endRange));
    }
    else
    {
        SymbolicRange & rangeStart = Rstart.getRange();
        if (rangeStart.isValid())
        {
            // Start is an iterator: for i=1:N, for j=i:N, ... in the second for "i" in "i:n" is an iterator
            if (endRange || Rend.getConstant().getGVNValue(getGVN(), endRange))
            {
                // Start is an iterator and End is GVN value
                res.setRange(SymbolicRange(getGVN(), rangeStart.getStart(), endRange));
            }
            else
            {
                SymbolicRange & rangeEnd = Rend.getRange();
                if (rangeEnd.isValid())
                {
                    // Start & End are iterators
                    res.setRange(SymbolicRange(getGVN(), rangeStart.getStart(), rangeEnd.getEnd()));
                }
            }
        }
        else if (startRange || Rstart.getConstant().getGVNValue(getGVN(), startRange))
        {
            // Start is a GVN value
            SymbolicRange & rangeEnd = Rend.getRange();
            if (rangeEnd.isValid())
            {
                // Start is a GVN value and End is an iterator
                res.setRange(SymbolicRange(getGVN(), startRange, rangeEnd.getEnd()));
            }
        }
    }

    setResult(res);
}
}
