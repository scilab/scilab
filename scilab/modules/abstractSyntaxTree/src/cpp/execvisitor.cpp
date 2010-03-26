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
#include "stack-def.h"

#include "execvisitor.hxx"

#include "timer.hxx"
#include "localization.h"

#include "yaspio.hxx"

using std::string;

namespace ast
{
	void ExecVisitor::visit (const SeqExp  &e)
	{
		std::cout << "ExecVisitor::SeqExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const MatrixExp &e)
	{
		std::cout << "ExecVisitor::MatrixExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const MatrixLineExp &e)
	{
		std::cout << "ExecVisitor::MatrixLineExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const CellExp &e)
	{
		std::cout << "ExecVisitor::CellExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const StringExp &e)
	{
		std::cout << "ExecVisitor::StringExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const CommentExp &e)
	{
		std::cout << "ExecVisitor::CommentExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const IntExp &e)
	{
		std::cout << "ExecVisitor::IntExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const FloatExp &e)
	{
		std::cout << "ExecVisitor::FloatExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const DoubleExp &e)
	{
		std::cout << "ExecVisitor::DoubleExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const BoolExp &e)
	{
		std::cout << "ExecVisitor::BoolExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const NilExp &e)
	{
		std::cout << "ExecVisitor::NilExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const SimpleVar &e)
	{
		std::cout << "ExecVisitor::SimpleVar" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const ColonVar &e)
	{
		std::cout << "ExecVisitor::ColonVar" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const DollarVar &e)
	{
		std::cout << "ExecVisitor::DollarVar" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const ArrayListVar &e)
	{
		std::cout << "ExecVisitor::ArrayListVar" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const FieldExp &e)
	{
		std::cout << "ExecVisitor::FieldExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const OpExp &e)
	{
		std::cout << "ExecVisitor::OpExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const LogicalOpExp &e)
	{
		std::cout << "ExecVisitor::LogicalOpExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const AssignExp &e)
	{
		std::cout << "ExecVisitor::AssignExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const CallExp &e)
	{
		std::cout << "ExecVisitor::CallExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const IfExp &e)
	{
		std::cout << "ExecVisitor::IfExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const TryCatchExp &e)
	{
		std::cout << "ExecVisitor::TryCatchExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const WhileExp &e)
	{
		std::cout << "ExecVisitor::SeqExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const ForExp &e)
	{
		std::cout << "ExecVisitor::ForExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const BreakExp &e)
	{
		std::cout << "ExecVisitor::BreakExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const ReturnExp &e)
	{
		std::cout << "ExecVisitor::ReturnExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const SelectExp &e)
	{
		std::cout << "ExecVisitor::SelectExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const CaseExp &e)
	{
		std::cout << "ExecVisitor::CaseExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const ArrayListExp &e)
	{
		std::cout << "ExecVisitor::ArrayListExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const AssignListExp &e)
	{
		std::cout << "ExecVisitor::AssignListExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const NotExp &e)
	{
		std::cout << "ExecVisitor::NotExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const TransposeExp &e)
	{
		std::cout << "ExecVisitor::TransposeExp" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const VarDec &e)
	{
		std::cout << "ExecVisitor::VarDec" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit (const FunctionDec &e)
	{
		std::cout << "ExecVisitor::FunctionDec" << std::endl;
		visitprivate(e);
	}

	void ExecVisitor::visit(const ListExp &e)
	{
		std::cout << "ExecVisitor::ListExp" << std::endl;
		visitprivate(e);
	}

/*protected real visit functions*/
}
