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

#include "mutevisitor.hxx"

namespace ast {

	void MuteVisitor::visit (const MatrixExp &e)
	{
		std::list<MatrixLineExp *>::const_iterator	row;
		for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
		{
			(*row)->mute();
			MuteVisitor mute;
			(*row)->accept(mute);
		}
	}

	void MuteVisitor::visit (const MatrixLineExp &e)
	{
		std::list<Exp *>::const_iterator	col;
		for (col = e.columns_get().begin() ; col != e.columns_get().end() ; ++col)
		{
			(*col)->mute();
			MuteVisitor mute;
			(*col)->accept(mute);
		}
	}

	void MuteVisitor::visit (const CellExp &e)
	{
		std::list<MatrixLineExp *>::const_iterator	row;
		for (row = e.lines_get().begin() ; row != e.lines_get().end() ; ++row )
		{
			(*row)->mute();
			MuteVisitor mute;
			(*row)->accept(mute);
		}
	}

	void MuteVisitor::visit (const StringExp &e)
	{
	}

	void MuteVisitor::visit (const CommentExp &e)
	{
	}

	void MuteVisitor::visit (const IntExp  &e)
	{
	}

	void MuteVisitor::visit (const FloatExp  &e)
	{
	}

	void MuteVisitor::visit (const DoubleExp  &e)
	{
	}

	void MuteVisitor::visit (const BoolExp  &e)
	{
	}

	void MuteVisitor::visit (const NilExp &e)
	{
	}

	void MuteVisitor::visit (const SimpleVar &e)
	{
	}

	void MuteVisitor::visit (const ColonVar &e)
	{
	}

	void MuteVisitor::visit (const DollarVar &e)
	{
	}

	void MuteVisitor::visit (const ArrayListVar &e)
	{
		std::list<Var *>::const_iterator var;
		for (var = e.vars_get().begin() ; var != e.vars_get().end() ; var++)
		{
			(*var)->mute();
			MuteVisitor mute;
			(*var)->accept(mute);
		}
	}

	void MuteVisitor::visit (const FieldExp &e)
	{
		const_cast<Exp*>(e.head_get())->mute();
		const_cast<Exp*>(e.tail_get())->mute();

		MuteVisitor mute;
		e.head_get()->accept(mute);
		e.tail_get()->accept(mute);
	}

	void MuteVisitor::visit(const OpExp &e)
	{
		const_cast<Exp&>(e.left_get()).mute();			
		const_cast<Exp&>(e.right_get()).mute();			

		MuteVisitor mute;
		e.left_get().accept(mute);
		e.right_get().accept(mute);
	}

	void MuteVisitor::visit(const LogicalOpExp &e)
	{
		const_cast<Exp&>(e.left_get()).mute();			
		const_cast<Exp&>(e.right_get()).mute();			

		MuteVisitor mute;
		e.left_get().accept(mute);
		e.right_get().accept(mute);
	}

	void MuteVisitor::visit (const AssignExp  &e)
	{
		const_cast<Exp&>(e.left_exp_get()).mute();			
		const_cast<Exp&>(e.right_exp_get()).mute();			

		MuteVisitor mute;
		e.left_exp_get().accept(mute);
		e.right_exp_get().accept(mute);
	}

	void MuteVisitor::visit(const CallExp &e)
	{
		std::list<Exp *>::const_iterator arg;
		for (arg = e.args_get().begin() ; arg != e.args_get().end() ; arg++)
		{
			(*arg)->mute();
			MuteVisitor mute;
			(*arg)->accept(mute);
		}
	}

	void MuteVisitor::visit (const IfExp  &e)
	{
		const_cast<Exp&>(e.test_get()).mute();			
		const_cast<Exp&>(e.then_get()).mute();			
		if(e.has_else())
		{
			const_cast<Exp&>(e.else_get()).mute();			
		}

		MuteVisitor mute;
		e.test_get().accept(mute);
		e.then_get().accept(mute);
		if(e.has_else())
		{
			e.else_get().accept(mute);
		}
	}

	void MuteVisitor::visit (const TryCatchExp  &e)
	{
		const_cast<SeqExp&>(e.try_get()).mute();			
		const_cast<SeqExp&>(e.catch_get()).mute();			

		MuteVisitor mute;
		e.try_get().accept(mute);
		e.catch_get().accept(mute);
	}

	void MuteVisitor::visit (const WhileExp  &e)
	{
		const_cast<Exp&>(e.test_get()).mute();			
		const_cast<Exp&>(e.body_get()).mute();			

		MuteVisitor mute;
		e.test_get().accept(mute);
		e.body_get().accept(mute);
	}

	void MuteVisitor::visit (const ForExp  &e)
	{
		const_cast<Exp&>(e.body_get()).mute();			

		MuteVisitor mute;
		e.body_get().accept(mute);
	}

	void MuteVisitor::visit (const BreakExp &e)
	{
	}

	void MuteVisitor::visit (const ReturnExp &e)
	{
	}

	void MuteVisitor::visit (const SeqExp  &e)
	{
		std::list<Exp *>::const_iterator seq;
		for (seq = e.exps_get().begin() ; seq != e.exps_get().end() ; seq++)
		{
			(*seq)->mute();
			MuteVisitor mute;
			(*seq)->accept(mute);
		}
	}

	void MuteVisitor::visit (const ArrayListExp  &e)
	{
		std::list<Exp *>::const_iterator exp;
		for (exp = e.exps_get().begin() ; exp != e.exps_get().end() ; exp++)
		{
			(*exp)->mute();
			MuteVisitor mute;
			(*exp)->accept(mute);
		}
	}

	void MuteVisitor::visit (const AssignListExp  &e)
	{
	}

	void MuteVisitor::visit (const NotExp &e)
	{
	}

	void MuteVisitor::visit (const TransposeExp &e)
	{
	}

	void MuteVisitor::visit (const VarDec  &e)
	{
	}

	void MuteVisitor::visit (const FunctionDec  &e)
	{
		const_cast<Exp&>(e.body_get()).mute();			

		MuteVisitor mute;
		e.body_get().accept(mute);
	}

	void MuteVisitor::visit(const ListExp &e)
	{
	}
}
