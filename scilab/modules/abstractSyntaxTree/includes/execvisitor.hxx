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

#ifndef AST_EXECVISITOR_HXX
# define AST_EXECVISITOR_HXX

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "alldec.hxx"
#include "alltypes.hxx"
#include "context.hxx"

using namespace types;

int GetIndexList(std::list<ast::Exp *> _plstArg, int** _piIndexSeq, int** _piMaxDim, InternalType *_pRefVar, int *_iDimSize);
void ExpandList(int ** _piList, int *_piListSize, int _iListSizeSize, int *_piResultList);
int GetVarMaxDim(types::InternalType *_pIT, int _iCurrentDim, int _iMaxDim);

namespace ast
{
	class EXTERN_AST ExecVisitor : public GenVisitor<const_kind>
	{
	public:
		ExecVisitor()
		{
			_excepted_result = -1;
			_result.push_back(NULL);
		}

		~ExecVisitor()
		{
			result_clear();
		}

		void result_clear()
		{
			for(int i = 0 ; i < _result.size() ; i++)
			{
				if(_result[i] != NULL && _result[i]->isDeletable() == true)
				{
					delete _result[i];
				}
			}
		}

		/** \name Visit Matrix Expressions nodes.
		** \{ */
	public :
		virtual void visit (const MatrixExp &e);
		virtual void visit (const MatrixLineExp &e);
		/** \} */

		/** \name Visit Cell Expressions nodes.
		** \{ */
	public :
		virtual void visit (const CellExp &e);
		/** \} */

		/** \name Visit Constant Expressions nodes.
		** \{ */
	public :
		virtual void visit (const StringExp &e);
		virtual void visit (const CommentExp &e);
		virtual void visit (const IntExp  &e);
		virtual void visit (const FloatExp  &e);
		virtual void visit (const DoubleExp  &e);
		virtual void visit (const BoolExp  &e);
		virtual void visit (const NilExp &e);
		/** \} */

		/** \name Visit Variable related nodes.
		** \{ */
		virtual void visit (const SimpleVar &e);
		virtual void visit (const ColonVar &e);
		virtual void visit (const DollarVar &e);
		virtual void visit (const ArrayListVar &e);
		/** \} */

		/** \name Visit Control Expressions or Instructions nodes.
		** \{ */
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
		/** \} */

		/** \name Visit Single Operation nodes.
		** \{ */
		virtual void visit (const NotExp &e);
		virtual void visit (const TransposeExp &e);
		/** \} */

		/** \name Visit Declaration nodes.
		** \{ */
		/** \brief Visit Var declarations. */
		virtual void visit (const VarDec  &e);
		virtual void visit (const FunctionDec  &e);
		/** \} */

		/** \name Visit Type dedicated Expressions related node.
		** \{ */
	public:
		virtual void visit(const ListExp &e);
		/** \} */
		int result_size_get(void);
		int expected_size_get(void);
		void expected_size_set(int _iSize);
		types::InternalType*		result_get(void);
		types::InternalType*		result_get(int _iPos);
		vector<types::InternalType*>* result_list_get();
		void	result_set(const types::InternalType *gtVal);
		void	result_set(int _iPos, const types::InternalType *gtVal);


		/*-------------.
		| Attributes.  |
		`-------------*/
	protected:
		vector<types::InternalType*>	_result;
		int _excepted_result;
	};
}
#endif // !AST_EXECVISITOR_HXX
