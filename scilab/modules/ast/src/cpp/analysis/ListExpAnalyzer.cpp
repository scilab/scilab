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
