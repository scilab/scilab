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
#include "gvn/SymbolicList.hxx"
#include "ForList.hxx"
#include "tools.hxx"

namespace analysis
{

bool SymbolicList::getType(GVN & gvn, TIType & type) const
{
    double dstart, dstep, dend;
    bool known = false;
    if (symbolic)
    {
        const MultivariatePolynomial & mpStart = *start.gvnVal->poly;
        const MultivariatePolynomial & mpStep = *step.gvnVal->poly;
        const MultivariatePolynomial & mpEnd = *end.gvnVal->poly;
        if (mpStart.isConstant() && mpStep.isConstant() && mpEnd.isConstant())
        {
            dstart = mpStart.constant;
            dstep = mpStep.constant;
            dend = mpEnd.constant;
            known = true;
        }
    }
    else
    {
        dstart = start.dval;
        dstep = step.dval;
        dend = end.dval;
        known = true;
    }

    if (known)
    {
        double out;
        int _type = ForList64::checkList(dstart, dend, dstep, out);

        switch (_type)
        {
            case 0:
                type = TIType(gvn, TIType::EMPTY);
                return true;
            case 1:
                type = TIType(gvn, TIType::DOUBLE);
                return true;
            case 2:
            {
                const uint64_t N = ForList64::size(dstart, dend, dstep);
                type = TIType(gvn, TIType::DOUBLE, 1, N);
                return true;
            }
            default:
                return false;
        }
    }

    GVN::Value * gvnStart = start.gvnVal, * gvnStep = step.gvnVal, * gvnEnd = end.gvnVal;
    if (!gvnStep->poly->isConstant())
    {
        return false;
    }

    dstep = gvnStep->poly->constant;
    if (dstep == 0)
    {
        type = TIType(gvn, TIType::EMPTY);
        return true;
    }

    if (dstep != -1 && dstep != 1)
    {
        // TODO : we must be able to handle general step (even if -1 or 1 seem to be the most frequent values)
        // but it implies that we need a symbolic division on polynomials.
        return false;
    }

    GVN::Value * ONEValue = gvn.getValue(1.);
    SymbolicDimension ONE(gvn, ONEValue);

    if (gvnStart->value == gvnEnd->value)
    {
        type = TIType(gvn, TIType::DOUBLE, ONE, ONE);
        return true;
    }

    GVN::Value * v;
    if (dstep == 1)
    {
        v = gvn.getValue(OpValue::Kind::MINUS, *gvnEnd, *gvnStart);
    }
    else
    {
        v = gvn.getValue(OpValue::Kind::MINUS, *gvnStart, *gvnEnd);
    }
    v = gvn.getValue(OpValue::Kind::PLUS, *v, *ONEValue);
    if (v->poly->constant < 0 && v->poly->isCoeffNegative(false))
    {
        type = TIType(gvn, TIType::EMPTY);
        return true;
    }

    type = TIType(gvn, TIType::DOUBLE, ONE, SymbolicDimension(gvn, v));
    return true;
}

bool SymbolicList::get(AnalysisVisitor & visitor, ast::ListExp & le, SymbolicList & sl)
{
    le.getStart().accept(visitor);
    Result Rstart = visitor.getResult();
    le.getEnd().accept(visitor);
    Result Rend = visitor.getResult();
    le.getStep().accept(visitor);
    Result & Rstep = visitor.getResult();

    double start, step, end;
    if (Rstart.getConstant().getDblValue(start) && Rstep.getConstant().getDblValue(step) && Rend.getConstant().getDblValue(end))
    {
        if (tools::isAnInt(start) && tools::isAnInt(step) && tools::isAnInt(end))
        {
            sl = SymbolicList(start, step, end);
            return true;
        }
        return false;
    }

    GVN::Value * gvnStart, * gvnStep, * gvnEnd;
    if (Rstart.getConstant().getDblValue(start))
    {
        int64_t _start;
        if (tools::asInteger(start, _start))
        {
            gvnStart = visitor.getGVN().getValue(_start);
        }
        else
        {
            return false;
        }
    }
    else if (GVN::Value * v = Rstart.getConstant().getGVNValue())
    {
        gvnStart = v;
    }
    else
    {
        return false;
    }

    if (Rstep.getConstant().getDblValue(step))
    {
        int64_t _step;
        if (tools::asInteger(step, _step))
        {
            gvnStep = visitor.getGVN().getValue(_step);
        }
        else
        {
            return false;
        }
    }
    else if (GVN::Value * v = Rstep.getConstant().getGVNValue())
    {
        gvnStep = v;
    }
    else
    {
        return false;
    }

    if (Rend.getConstant().getDblValue(end))
    {
        int64_t _end;
        if (tools::asInteger(end, _end))
        {
            gvnEnd = visitor.getGVN().getValue(_end);
        }
        else
        {
            return false;
        }
    }
    else if (GVN::Value * v = Rend.getConstant().getGVNValue())
    {
        gvnEnd = v;
    }
    else
    {
        return false;
    }

    sl = SymbolicList(gvnStart, gvnStep, gvnEnd);

    return true;
}

void SymbolicList::evalDollar(GVN & gvn, const GVN::Value * dollarVal)
{
    if (GVN::Value * const dollar = gvn.getExistingValue(symbol::Symbol(L"$")))
    {
        if (GVN::Value * v = evalDollar(gvn, getStart(), dollar, dollarVal))
        {
            setStart(v);
        }

        if (GVN::Value * v = evalDollar(gvn, getStep(), dollar, dollarVal))
        {
            setStep(v);
        }

        if (GVN::Value * v = evalDollar(gvn, getEnd(), dollar, dollarVal))
        {
            setEnd(v);
        }
    }
}

bool SymbolicList::checkAsIndex(const GVN::Value * dim)
{
    if (symbolic)
    {
        if (getStep()->poly->constant > 0 && getStep()->poly->isCoeffPositive(false))
        {
            // step is positive

        }
        else if (getStep()->poly->constant < 0 && getStep()->poly->isCoeffNegative(false))
        {
            // step is negative

        }
        else
        {

        }
    }
    else
    {

    }

    return true;
}
}
