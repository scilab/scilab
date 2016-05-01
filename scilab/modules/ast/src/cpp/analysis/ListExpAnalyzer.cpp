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

namespace analysis
{
bool ListExpAnalyzer::analyze(ast::ListExp & e, SymbolicList & sl, TIType & type)
{
    e.getStart().accept(*this);
    Result Rstart = getResult();
    e.getEnd().accept(*this);
    Result Rend = getResult();
    e.getStep().accept(*this);
    Result & Rstep = getResult();

    double start, step, end;
    if (Rstart.getConstant().getDblValue(start) && Rstep.getConstant().getDblValue(step) && Rend.getConstant().getDblValue(end))
    {
        double out;
        int type = ForList64::checkList(start, end, step, out);

        switch (type)
        {
            case 0:
                type = TIType(dm.getGVN(), TIType::EMPTY);
                break;
            case 1:
                type = TIType(dm.getGVN(), TIType::DOUBLE);
                break;
            case 2:
            {
                const uint64_t N = ForList64::size(start, end, step);
                type = TIType(getGVN(), TIType::DOUBLE, 1, N);
                sl = SymbolicList(start, step, end);
                break;
            }
            default:
                break;
        }
        e.setValues(start, step, end, out);
        return true;
    }

    GVN::Value * gvnStart, * gvnStep, * gvnEnd;
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
            Result & res = e.getDecorator().setResult(Result(TIType(dm.getGVN(), Rstart.getType().type, false, true), -1));
            setResult(res);
            return;
        }
    }

    GVN & gvn = getGVN();
    if (Rstart.getConstant().getGVNValue(gvn, gvnStart) && Rstep.getConstant().getGVNValue(gvn, gvnStep) && Rend.getConstant().getGVNValue(gvn, gvnEnd))
    {
        sl = SymbolicList(gvnStart, gvnStep, gvnEnd);
        if (gvnStep->poly->isConstant())
        {
            step = gvnStep->poly->constant;
            if (step == 1 || step == -1)
            {

            }
        }
    }
}
}
