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

#include "shortcutvisitor.hxx"
#include "timer.hxx"
#include "localization.h"

#include "yaspio.hxx"

using std::string;

namespace ast
{
	void ShortCutVisitor::visit (const MatrixLineExp &e)
	{
		std::list<Exp*>::const_iterator it;
		for(it = e.columns_get().begin() ; it != e.columns_get().end() ; it++)
		{
			ShortCutVisitor SC;
			(*it)->accept(SC);
		}
	}

	void ShortCutVisitor::visit (const CellExp &e)
	{
		std::list<MatrixLineExp*>::const_iterator it;
		for(it = e.lines_get().begin() ; it != e.lines_get().end() ; it++)
		{
			ShortCutVisitor SC;
			(*it)->accept(SC);
		}
	}

	void ShortCutVisitor::visit (const StringExp &e)
	{
	}

	void ShortCutVisitor::visit (const CommentExp &e)
	{
	}

	void ShortCutVisitor::visit (const IntExp  &e)
	{
	}

	void ShortCutVisitor::visit (const FloatExp  &e)
	{
	}

	void ShortCutVisitor::visit (const DoubleExp  &e)
	{
	}

	void ShortCutVisitor::visit (const BoolExp  &e)
	{
	}

	void ShortCutVisitor::visit (const NilExp &e)
	{
	}

	void ShortCutVisitor::visit (const SimpleVar &e)
	{
	}

	void ShortCutVisitor::visit (const ColonVar &e)
	{
	}

	void ShortCutVisitor::visit (const DollarVar &e)
	{
	}

	void ShortCutVisitor::visit (const ArrayListVar &e)
	{
	}

	void ShortCutVisitor::visit (const FieldExp &e)
	{
		ShortCutVisitor SCHead;
		e.head_get()->accept(SCHead);

		ShortCutVisitor SCTail;
		e.tail_get()->accept(SCTail);
	}

	void ShortCutVisitor::visit(const CallExp &e)
	{
		std::list<Exp*>::const_iterator it;
		for(it = e.args_get().begin() ; it != e.args_get().end() ; it++)
		{
			ShortCutVisitor SC;
			(*it)->accept(SC);
		}
	}

	void ShortCutVisitor::visit (const IfExp  &e)
	{
		ShortCutVisitor SCTest;
		e.test_get().accept(SCTest);

		ShortCutVisitor SCThen;
		e.then_get().accept(SCThen);

		if(e.has_else())
		{
			ShortCutVisitor SCElse;
			e.else_get().accept(SCElse);
		}
	}

	void ShortCutVisitor::visit (const TryCatchExp  &e)
	{
		ShortCutVisitor SCTry;
		e.try_get().accept(SCTry);

		ShortCutVisitor SCCatch;
		e.catch_get().accept(SCCatch);
	}

	void ShortCutVisitor::visit (const WhileExp  &e)
	{
		ShortCutVisitor SCTest;
		e.test_get().accept(SCTest);

		ShortCutVisitor SCBody;
		e.body_get().accept(SCBody);
	}

	void ShortCutVisitor::visit (const ForExp  &e)
	{
		ShortCutVisitor SCBody;
		e.body_get().accept(SCBody);
	}

	void ShortCutVisitor::visit (const BreakExp &e)
	{
	}

	void ShortCutVisitor::visit (const ReturnExp &e)
	{
	}

	void ShortCutVisitor::visit (const SelectExp &e)
	{
	}

	void ShortCutVisitor::visit (const CaseExp &e)
	{
	}

	void ShortCutVisitor::visit (const SeqExp  &e)
	{
		std::list<Exp*>::const_iterator it;
		for(it = e.exps_get().begin() ; it != e.exps_get().end() ; it++)
		{
			ShortCutVisitor SC;
			(*it)->accept(SC);
		}
	}

	void ShortCutVisitor::visit (const ArrayListExp  &e)
	{
		std::list<Exp*>::const_iterator it;
		for(it = e.exps_get().begin() ; it != e.exps_get().end() ; it++)
		{
			ShortCutVisitor SC;
			(*it)->accept(SC);
		}
	}

	void ShortCutVisitor::visit (const AssignListExp  &e)
	{
		std::list<Exp*>::const_iterator it;
		for(it = e.exps_get().begin() ; it != e.exps_get().end() ; it++)
		{
			ShortCutVisitor SC;
			(*it)->accept(SC);
		}
	}

	void ShortCutVisitor::visit (const NotExp &e)
	{
	}

	void ShortCutVisitor::visit (const TransposeExp &e)
	{
	}

	void ShortCutVisitor::visit (const VarDec  &e)
	{
	}

	void ShortCutVisitor::visit (const FunctionDec  &e)
	{
		ShortCutVisitor SCBody;
		e.body_get().accept(SCBody);
	}
	
	void ShortCutVisitor::visit (const ClassDec &e)
	{
		slots_t::const_iterator it;
		for(it = e.slots_get().begin() ; it != e.slots_get().end() ; ++it)
		{
			ShortCutVisitor SCSlot;
			(*it)->accept(SCSlot);
		}
	}
	
	void ShortCutVisitor::visit (const PropertyDec &e)
	{
		if(e.default_get())
		{
			ShortCutVisitor SCProperty;
			e.default_get()->accept(SCProperty);
		}
	}
	
	void ShortCutVisitor::visit (const MethodDec &e)
	{
		ShortCutVisitor SCMethod;
		e.body_get().accept(SCMethod);
	}

	void ShortCutVisitor::visit(const ListExp &e)
	{
		ShortCutVisitor SCStart;
		e.start_get().accept(SCStart);

		ShortCutVisitor SCStep;
		e.step_get().accept(SCStep);

		ShortCutVisitor SCEnd;
		e.end_get().accept(SCEnd);
	}

	void ShortCutVisitor::visit(const MatrixExp &e)
	{
		std::list<MatrixLineExp*>::const_iterator it;
		for(it = e.lines_get().begin() ; it != e.lines_get().end() ; it++)
		{
			ShortCutVisitor SC;
			(*it)->accept(SC);
		}
	}

	void ShortCutVisitor::visit(const AssignExp &e)
	{
		ShortCutVisitor SCLeft;
		e.left_exp_get().accept(SCLeft);

		ShortCutVisitor SCRight;
		e.right_exp_get().accept(SCRight);
	}

	void ShortCutVisitor::visit(const LogicalOpExp &e)
	{
		ShortCutVisitor SCLeft;
		e.left_get().accept(SCLeft);

		ShortCutVisitor SCRight;
		e.right_get().accept(SCRight);

		switch(e.oper_get())
		{
		case LogicalOpExp::logicalOr :
			((LogicalOpExp*)&e)->oper_set(LogicalOpExp::logicalShortCutOr);
			break;
		case LogicalOpExp::logicalAnd :
			((LogicalOpExp*)&e)->oper_set(LogicalOpExp::logicalShortCutAnd);
			break;
		}
	}

	void ShortCutVisitor::visit(const OpExp &e)
	{
		ShortCutVisitor SCLeft;
		e.left_get().accept(SCLeft);

		ShortCutVisitor SCRight;
		e.right_get().accept(SCRight);
	}
}
