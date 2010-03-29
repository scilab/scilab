/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "timedvisitor.hxx"

#include "timer.hxx"
#include "localization.h"

#include "yaspio.hxx"

using std::string;

namespace ast
{
	void TimedVisitor::visit (const SeqExp &e)
	{
		Timer timer;
		timer.start();
		visitprivate(e);
		((Ast&)e).elapsedtime_set(timer.elapsed_time());
	}

	void TimedVisitor::visit (const MatrixExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const MatrixLineExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const CellExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const StringExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const CommentExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const IntExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const FloatExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const DoubleExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const BoolExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const NilExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const SimpleVar &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const ColonVar &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const DollarVar &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const ArrayListVar &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const FieldExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const OpExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const LogicalOpExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const AssignExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const CallExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const IfExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const TryCatchExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const WhileExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const ForExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const BreakExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const ReturnExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const SelectExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const CaseExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const ArrayListExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const AssignListExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const NotExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const TransposeExp &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const VarDec &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit (const FunctionDec &e)
	{
		visitprivate(e);
	}

	void TimedVisitor::visit(const ListExp &e)
	{
		visitprivate(e);
	}
}
