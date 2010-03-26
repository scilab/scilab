/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef AST_TIMEDVISITOR_HXX
#define AST_TIMEDVISITOR_HXX

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>

#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"
#include "alltypes.hxx"
#include "runvisitor.hxx"

namespace ast
{
	class EXTERN_AST TimedVisitor : public RunVisitorT<TimedVisitor>
	{
	public :
		virtual void visit (const MatrixExp &e);
		virtual void visit (const MatrixLineExp &e);
		virtual void visit (const CellExp &e);
		virtual void visit (const StringExp &e);
		virtual void visit (const CommentExp &e);
		virtual void visit (const IntExp  &e);
		virtual void visit (const FloatExp  &e);
		virtual void visit (const DoubleExp  &e);
		virtual void visit (const BoolExp  &e);
		virtual void visit (const NilExp &e);
		virtual void visit (const SimpleVar &e);
		virtual void visit (const ColonVar &e);
		virtual void visit (const DollarVar &e);
		virtual void visit (const ArrayListVar &e);
		virtual void visit (const FieldExp &e);
		virtual void visit (const OpExp &e);
		virtual void visit (const LogicalOpExp &e);
		virtual void visit (const AssignExp  &e);
		virtual void visit (const CallExp &e);
		virtual void visit (const IfExp  &e);
		virtual void visit (const TryCatchExp  &e);
		virtual void visit (const WhileExp  &e);
		virtual void visit (const ForExp  &e);
		virtual void visit (const BreakExp &e);
		virtual void visit (const ReturnExp &e);
		virtual void visit (const SelectExp &e);
		virtual void visit (const CaseExp &e);
		virtual void visit (const SeqExp  &e);
		virtual void visit (const ArrayListExp  &e);
		virtual void visit (const AssignListExp  &e);
		virtual void visit (const NotExp &e);
		virtual void visit (const TransposeExp &e);
		virtual void visit (const VarDec  &e);
		virtual void visit (const FunctionDec  &e);
		virtual void visit(const ListExp &e);
	};
}
#endif // !AST_TIMEDVISITOR_HXX
