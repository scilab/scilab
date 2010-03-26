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
	void TimedVisitor::visit (const SeqExp  &e)
	{
		std::cout << "TimedVisitor::SeqExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const MatrixExp &e)
	{
		std::cout << "TimedVisitor::MatrixExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const MatrixLineExp &e)
	{
		std::cout << "TimedVisitor::MatrixLineExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const CellExp &e)
	{
		std::cout << "TimedVisitor::CellExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const StringExp &e)
	{
		std::cout << "TimedVisitor::StringExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const CommentExp &e)
	{
		std::cout << "TimedVisitor::CommentExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const IntExp &e)
	{
		std::cout << "TimedVisitor::IntExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const FloatExp &e)
	{
		std::cout << "TimedVisitor::FloatExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const DoubleExp &e)
	{
		std::cout << "TimedVisitor::DoubleExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const BoolExp &e)
	{
		std::cout << "TimedVisitor::BoolExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const NilExp &e)
	{
		std::cout << "TimedVisitor::NilExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const SimpleVar &e)
	{
		std::cout << "TimedVisitor::SimpleVar" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const ColonVar &e)
	{
		std::cout << "TimedVisitor::ColonVar" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const DollarVar &e)
	{
		std::cout << "TimedVisitor::DollarVar" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const ArrayListVar &e)
	{
		std::cout << "TimedVisitor::ArrayListVar" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const FieldExp &e)
	{
		std::cout << "TimedVisitor::FieldExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const OpExp &e)
	{
		std::cout << "TimedVisitor::OpExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const LogicalOpExp &e)
	{
		std::cout << "TimedVisitor::LogicalOpExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const AssignExp &e)
	{
		std::cout << "TimedVisitor::AssignExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const CallExp &e)
	{
		std::cout << "TimedVisitor::CallExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const IfExp &e)
	{
		std::cout << "TimedVisitor::IfExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const TryCatchExp &e)
	{
		std::cout << "TimedVisitor::TryCatchExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const WhileExp &e)
	{
		std::cout << "TimedVisitor::SeqExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const ForExp &e)
	{
		std::cout << "TimedVisitor::ForExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const BreakExp &e)
	{
		std::cout << "TimedVisitor::BreakExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const ReturnExp &e)
	{
		std::cout << "TimedVisitor::ReturnExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const SelectExp &e)
	{
		std::cout << "TimedVisitor::SelectExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const CaseExp &e)
	{
		std::cout << "TimedVisitor::CaseExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const ArrayListExp &e)
	{
		std::cout << "TimedVisitor::ArrayListExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const AssignListExp &e)
	{
		std::cout << "TimedVisitor::AssignListExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const NotExp &e)
	{
		std::cout << "TimedVisitor::NotExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const TransposeExp &e)
	{
		std::cout << "TimedVisitor::TransposeExp" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const VarDec &e)
	{
		std::cout << "TimedVisitor::VarDec" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit (const FunctionDec &e)
	{
		std::cout << "TimedVisitor::FunctionDec" << std::endl;
		visitprivate(e);
	}

	void TimedVisitor::visit(const ListExp &e)
	{
		std::cout << "TimedVisitor::ListExp" << std::endl;
		visitprivate(e);
	}
}
