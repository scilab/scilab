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

#include <cstdio>
#include <numeric>
#include <iostream>
//#include "stack-def.h"

#include "execvisitor.hxx"

#include "timer.hxx"
#include "localization.h"

#include "yaspio.hxx"

using std::string;

namespace ast
{
	void ExecVisitor::visit (const SeqExp  &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const MatrixExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const MatrixLineExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const CellExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const StringExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const CommentExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const IntExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const FloatExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const DoubleExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const BoolExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const NilExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const SimpleVar &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const ColonVar &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const DollarVar &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const ArrayListVar &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const FieldExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const OpExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const LogicalOpExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const AssignExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const CellCallExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const CallExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const IfExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const TryCatchExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const WhileExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const ForExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const BreakExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const ReturnExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const SelectExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const CaseExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const ArrayListExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const AssignListExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const NotExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const TransposeExp &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const VarDec &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit (const FunctionDec &e)
	{
		visitprivate(e);
	}

	void ExecVisitor::visit(const ListExp &e)
	{
		visitprivate(e);
	}

/*protected real visit functions*/
}
