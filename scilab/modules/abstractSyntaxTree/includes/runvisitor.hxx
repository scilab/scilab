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

#ifndef AST_RUNVISITOR_HXX
# define AST_RUNVISITOR_HXX

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>

#include "all.hxx"
#include "types.hxx"


namespace ast
{
	class EXTERN_AST RunVisitor : public ConstVisitor
	{
	};

	template <class T> class EXTERN_AST RunVisitorT : public RunVisitor
	{
	public:
		RunVisitorT()
		{
			_excepted_result = -1;
			_resultVect.push_back(NULL);
			_result = NULL;
			m_bSingleResult = true;
		}

		~RunVisitorT()
		{
			result_clear();
		}

		void result_clear()
		{
			if(is_single_result())
			{
				if(_result != NULL && _result->isDeletable() == true)
				{
//					std::cout << "before single delete : " << _result << std::endl;
					delete _result;
//					std::cout << "after single delete" << std::endl;
				}
			}
			else
			{
				for(unsigned int i = 0 ; i < _resultVect.size() ; i++)
				{
					if(_resultVect[i] != NULL && _resultVect[i]->isDeletable() == true)
					{
						delete _resultVect[i];
					}
				}
			}
		}

	protected :
		int GetIndexList(std::list<ast::Exp *>const& _plstArg, int** _piIndexSeq, int** _piMaxDim, types::InternalType *_pRefVar, int *_iDimSize);

		/** \name Visit Matrix Expressions nodes.
		** \{ */
	public :
		 virtual void visitprivate(const MatrixExp &e);
		 virtual void visitprivate(const MatrixLineExp &e);
		/** \} */

		/** \name Visit Cell Expressions nodes.
		** \{ */
	public :
		 virtual void visitprivate(const CellExp &e);
		/** \} */

		/** \name Visit Constant Expressions nodes.
		** \{ */
	public :
		 virtual void visitprivate(const StringExp &e);
		 virtual void visitprivate(const CommentExp &e);
		 virtual void visitprivate(const IntExp  &e);
		 virtual void visitprivate(const FloatExp  &e);
		 virtual void visitprivate(const DoubleExp  &e);
		 virtual void visitprivate(const BoolExp  &e);
		 virtual void visitprivate(const NilExp &e);
		/** \} */

		/** \name Visit Variable related nodes.
		** \{ */
		 virtual void visitprivate(const SimpleVar &e);
		 virtual void visitprivate(const ColonVar &e);
		 virtual void visitprivate(const DollarVar &e);
		 virtual void visitprivate(const ArrayListVar &e);
		/** \} */

		/** \name Visit Control Expressions or Instructions nodes.
		** \{ */
		 virtual void visitprivate(const FieldExp &e);
		 virtual void visitprivate(const OpExp &e);
		 virtual void visitprivate(const LogicalOpExp &e);
		 virtual void visitprivate(const AssignExp  &e);
		 virtual void visitprivate(const CallExp &e);
		 virtual void visitprivate(const IfExp  &e);
		 virtual void visitprivate(const TryCatchExp  &e);
		 virtual void visitprivate(const WhileExp  &e);
		 virtual void visitprivate(const ForExp  &e);
		 virtual void visitprivate(const BreakExp &e);
		 virtual void visitprivate(const ReturnExp &e);
		 virtual void visitprivate(const SelectExp &e);
		 virtual void visitprivate(const CaseExp &e);
		 virtual void visitprivate(const SeqExp  &e);
		 virtual void visitprivate(const ArrayListExp  &e);
		 virtual void visitprivate(const AssignListExp  &e);
		/** \} */

		/** \name Visit Single Operation nodes.
		** \{ */
		 virtual void visitprivate(const NotExp &e);
		 virtual void visitprivate(const TransposeExp &e);
		/** \} */

		/** \name Visit Declaration nodes.
		** \{ */
		/** \brief Visit Var declarations. */
		 virtual void visitprivate(const VarDec  &e);
		 virtual void visitprivate(const FunctionDec  &e);
		/** \} */

		/** \name Visit Type dedicated Expressions related node.
		** \{ */
	public:
		 virtual void visitprivate(const ListExp &e);
		/** \} */

		int expected_size_get(void)
		{
			return _excepted_result;
		}

		int result_size_get(void)
		{
			if(is_single_result())
			{
				if(_result == NULL)
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}
			else
			{
			  return static_cast<int>(_resultVect.size());
			}
		}

		void expected_size_set(int _iSize)
		{
			_excepted_result = _iSize;
		}

		types::InternalType* result_get(void)
		{
			if(is_single_result())
			{
				return _result;
			}
			else
			{
				return _resultVect[0];
			}
		}

		types::InternalType* result_get(int _iPos)
		{
			if(_iPos >= _resultVect.size())
			{
				return NULL;
			}
			return _resultVect[_iPos];
		}

		vector<types::InternalType*>* result_list_get()
		{
			return &_resultVect;
		}

		void result_set(int _iPos, const types::InternalType *gtVal)
		{
			m_bSingleResult = false;
			if(_iPos < _resultVect.size())
			{
				if(_resultVect[_iPos] != NULL && _resultVect[_iPos]->isDeletable())
				{
					delete _resultVect[_iPos];
				}
			}

			if(_iPos >= _resultVect.size())
			{
				_resultVect.resize(_iPos + 1, NULL);
			}

			_resultVect[_iPos] = const_cast<types::InternalType *>(gtVal);
		}

		void result_set(const types::InternalType *gtVal)
		{
			m_bSingleResult = true;
			_result = const_cast<types::InternalType *>(gtVal);
		}

		bool is_single_result()
		{
			return m_bSingleResult;
		}

		/*
		int result_size_get(void);
		int expected_size_get(void);
		void expected_size_set(int _iSize);
		types::InternalType*		result_get(void);
		types::InternalType*		result_get(int _iPos);
		vector<types::InternalType*>* result_list_get();
		void	result_set(const types::InternalType *gtVal);
		void	result_set(int _iPos, const types::InternalType *gtVal);
		bool	is_single_result();
		*/

		/*-------------.
		| Attributes.  |
		`-------------*/
	protected:
		vector<types::InternalType*>	_resultVect;
		types::InternalType*	_result;
		bool m_bSingleResult;
		int _excepted_result;
	//private :
	//	RunVisitorT(RunVisitorT const& e){}
	};
}

#endif // !AST_RUNVISITOR_HXX
