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

#include "execvisitor.hxx"
#include "timer.hxx"
using std::string;

bool bConditionState(ast::ExecVisitor *exec);

void vTransposeRealMatrix(
			double *_pdblRealIn,
			int _iRowsIn,			int _iColsIn,
			double *_pdblRealOut);

void vTransposeComplexMatrix(
			double *_pdblRealIn,	double *_pdblImgIn,
			int _iRowsIn,			int _iColsIn,
			double *_pdblRealOut,	double *_pdblImgOut, bool _bConjugate);

namespace ast
{
	static int level = -1;

	static void DEBUG_START_NODE(void)
	{
		++level;
	}

	static void DEBUG_END_NODE(void)
	{
		--level;
	}

	static void DEBUG(string str)
	{
		for(int i = 0 ; i < level; ++i)
		{
			std::cerr << "  ";
		}
		if (level > 0)
		{
			std::cerr << "|_./ ";
		}
		std::cerr << str << std::endl;
	}

	void ExecVisitor::visit (const MatrixLineExp &e)
	{
	/*
		All processes are done in MatrixExp
	*/
	}

	void ExecVisitor::visit (const CellExp &e)
	{
		/*
			FIXME : container type
		*/
	}

	/** \name Visit Constant Expressions nodes.
	** \{ */
	void ExecVisitor::visit (const StringExp &e)
	{
		String *psz = new String(e.value_get().c_str());
		result_set(psz);
	}

	void ExecVisitor::visit (const CommentExp &e)
	{
		/*
		Nothing to do
		*/
	}

	void ExecVisitor::visit (const IntExp  &e)
	{
		/*
		Int does not exist, Int8 - 16 - 32 - 64 functions
		*/
	}

	void ExecVisitor::visit (const FloatExp  &e)
	{
		/*
		Float does not exist, float function
		*/
	}

	void ExecVisitor::visit (const DoubleExp  &e)
	{
		Double *pdbl = new Double(e.value_get());
		result_set(pdbl);
	}

	void ExecVisitor::visit (const BoolExp  &e)
	{
		Bool *pb = new Bool(e.value_get());
		result_set(pb);
	}

	void ExecVisitor::visit (const NilExp &e)
	{
		/*
		FIXME :
		*/
	}

	void ExecVisitor::visit (const SimpleVar &e)
	{
		result_set(symbol::Context::getInstance()->get(e.name_get()));
	}

	void ExecVisitor::visit (const ColonVar &e)
	{
		/*
		: = 1:$
		*/
	}

	void ExecVisitor::visit (const DollarVar &e)
	{
		int pRank[1] = {2};
		Double dblCoef(1,2);
		dblCoef.val_set(0, 0, 0);
		dblCoef.val_set(0, 1, 1);

		MatrixPoly *pVar	= new MatrixPoly("$", 1, 1, pRank);
		Poly *poPoly			= pVar->poly_get(0,0);
		poPoly->coef_set(&dblCoef);
		result_set(pVar);
	}

	void ExecVisitor::visit (const ArrayListVar &e)
	{
		/*

		*/
	}

	void ExecVisitor::visit (const FieldExp &e)
	{
		/*
		a.b
		*/
	}

	void ExecVisitor::visit(const CallExp &e)
	{
		ExecVisitor *execVar	= new ast::ExecVisitor();
		ExecVisitor *execFunc = new ast::ExecVisitor();
		std::list<Exp *>::const_iterator	i;

		e.name_get().accept(*execFunc);
		if(execFunc->result_get() != NULL && execFunc->result_get()->getType() == InternalType::RealFunction)
		{//function call
			Function *pF = execFunc->result_get()->getAsFunction();
			types::typed_list out;

			types::typed_list in;
			for (i = e.args_get().begin (); i != e.args_get().end (); ++i)
			{
				(*i)->accept (*execVar);
				in.push_back(execVar->result_get());
			}

			Function::ReturnValue Ret = pF->m_pFunc(in, 0, out);
			if(Ret == Function::AllGood)
			{
				result_set(out.front()->getAsDouble());
			}
		}
		else if(execFunc->result_get() != NULL)
		{//a(xxx) with a variable

			//get symbol of variable
			const SimpleVar *Var = dynamic_cast<const SimpleVar*>(&e.name_get());

			InternalType *pData = NULL;
			if(execFunc->result_get()->getType() == InternalType::RealDouble)
			{
				pData = execFunc->result_get()->getAsDouble();
			}

			//visit each args
			typed_list in;
			for (i = e.args_get().begin (); i != e.args_get().end (); ++i)
			{
				(*i)->accept (*execVar);
				in.push_back(execVar->result_get());
			}

			typed_list::const_iterator	j;
			for(j = in.begin() ; j != in.end() ; j++)
			{
				if((*j)->getType() == InternalType::RealImplicitList)
				{//ca pue la mort ca !

				}

			}
		}
		else
		{//result == NULL ,variable doesn't exist :(
		}


		delete execVar;
		delete execFunc;
	}

	void ExecVisitor::visit (const IfExp  &e)
	{
			//Create local exec visitor
			ExecVisitor *execMeTest		= new ast::ExecVisitor();
			ExecVisitor *execMeAction = new ast::ExecVisitor();
			bool bTestStatus					= false;

			//condition
			e.test_get().accept(*execMeTest);

			bTestStatus = bConditionState(execMeTest);
			if(bTestStatus == true)
			{//condition == true
				e.then_get().accept(*execMeAction);
			}
			else
			{//condition == false
				e.else_get().accept(*execMeAction);
			}
		delete execMeAction;
		delete execMeTest;
	}

	void ExecVisitor::visit (const TryCatchExp  &e)
	{
	}

	void ExecVisitor::visit (const WhileExp  &e)
	{
			ExecVisitor *execMeTest		= new ast::ExecVisitor();
			ExecVisitor *execMeAction = new ast::ExecVisitor();
			bool bTestStatus					= false;

			//condition
			e.test_get().accept(*execMeTest);
			while(bConditionState(execMeTest))
			{
				e.body_get().accept(*execMeAction);
				e.test_get().accept(*execMeTest);
			}
	}

	void ExecVisitor::visit (const ForExp  &e)
	{
		ExecVisitor *execMe = new ast::ExecVisitor();
		e.vardec_get().accept(*execMe);

		if(execMe->result_get()->getType() == InternalType::RealImplicitList)
		{
			ImplicitList* pVar = (ImplicitList*)execMe->result_get();
			for(int i = 0 ; i < pVar->size_get() ; i++)
			{
				double dblVal = pVar->extract_value(i);
				Double *pdbl = new Double(dblVal);
				symbol::Context::getInstance()->put(e.vardec_get().name_get(), *(GenericType*)pdbl);
				e.body_get().accept(*execMe);
			}
		}
		else
		{//Matrix i = [1,3,2,6] or other type

		}

		delete execMe;
	}

	void ExecVisitor::visit (const BreakExp &e)
	{
	}

	void ExecVisitor::visit (const ReturnExp &e)
	{
	}

	void ExecVisitor::visit (const SeqExp  &e)
	{
		ExecVisitor *execMe = new ast::ExecVisitor();
		std::list<Exp *>::const_iterator	i;

		for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
		{
			(*i)->accept (*execMe);
		}

		delete execMe;
	}

	void ExecVisitor::visit (const ArrayListExp  &e)
	{
	}

	void ExecVisitor::visit (const AssignListExp  &e)
	{
	}
	/** \} */

	/** \name Visit Single Operation nodes.
	** \{ */
	void ExecVisitor::visit (const NotExp &e)
	{
		/*
		@ or ~= !
		*/
		ExecVisitor* execMe = new ast::ExecVisitor();
		e.exp_get().accept(*execMe);

		if(execMe->result_get()->isDouble())
		{
			Double *pdbl	= execMe->result_get()->getAsDouble();
			Bool *pReturn	= new Bool(pdbl->rows_get(), pdbl->cols_get());
			double *pR		= pdbl->real_get();
			bool *pB			= pReturn->bool_get();
			for(int i = 0 ; i < pdbl->size_get() ; i++)
			{
				if(pR[i] != 0)
				{
					pB[i] = true;
				}
				else
				{
					pB[i] = false;
				}
			}
			result_set(pReturn);
		}
		else if(execMe->result_get()->isBool())
		{
			Bool *pb			= execMe->result_get()->getAsBool();
			Bool *pReturn	= new Bool(pb->rows_get(), pb->cols_get());
			bool *pR			= pb->bool_get();
			bool *pB			= pReturn->bool_get();

			for(int i = 0 ; i < pb->size_get() ; i++)
			{
				pB[i] = pR[i];
			}
			result_set(pReturn);
		}
		delete execMe;
	}

	void ExecVisitor::visit (const TransposeExp &e)
	{
		/*
		'
		*/
		ExecVisitor* execMe = new ast::ExecVisitor();
		e.exp_get().accept(*execMe);

		bool bConjug = e.conjugate_get() == TransposeExp::_Conjugate_;

		if(execMe->result_get()->isDouble())
		{
			Double *pdbl		= execMe->result_get()->getAsDouble();
			Double *pReturn	= NULL;

			if(pdbl->isComplex())
			{
				pReturn				= new Double(pdbl->cols_get(), pdbl->rows_get(), true);
				double *pInR	=	pdbl->real_get();
				double *pInI	=	pdbl->img_get();
				double *pOutR	=	pReturn->real_get();
				double *pOutI	=	pReturn->img_get();

				vTransposeComplexMatrix(pInR, pInI, pdbl->rows_get(), pdbl->cols_get(), pOutR, pOutI, bConjug);
			}
			else
			{
				pReturn				= new Double(pdbl->cols_get(), pdbl->rows_get(), false);
				double *pInR	=	pdbl->real_get();
				double *pOutR	=	pReturn->real_get();

				vTransposeRealMatrix(pInR, pdbl->rows_get(), pdbl->cols_get(), pOutR);
			}
			result_set(pReturn);
		}
		else
		{
		}



		delete execMe;
	}
	/** \} */

	/** \name Visit Declaration nodes.
	** \{ */
	/** \brief Visit Var declarations. */
	void ExecVisitor::visit (const VarDec  &e)
	{
		/*Create local exec visitor*/
		ExecVisitor* execMe = new ast::ExecVisitor();
		try
		{
			/*getting what to assign*/
			e.init_get().accept(*execMe);
			result_set(execMe->result_get());
		}
		catch(string sz)
		{
			throw sz;
		}
		delete execMe;
	}

	void ExecVisitor::visit (const FunctionDec  &e)
	{
		/*
		function foo
		endfunction
		*/

	}
	/** \} */

	/** \name Visit Type dedicated Expressions related node.
	** \{ */
	void ExecVisitor::visit(const ListExp &e)
	{
			double dblStart = -1;
			double dblStep = -1;
			double dblEnd = -1;

			ExecVisitor*	execMeStart = new ast::ExecVisitor();
			ExecVisitor*	execMeStep	= new ast::ExecVisitor();
			ExecVisitor*	execMeEnd		= new ast::ExecVisitor();


			e.start_get().accept(*execMeStart);
/*			if(execMeStart->result_get()->isDouble())
			{
				pIL->start_set(((Double*)execMeStart->result_get())->real_get(0,0));
			}
			else if(execMeStart->result_get()->isPoly())
			{
				pIL->start_set(((Poly*)execMeStart->result_get()));
			}
*/

			e.step_get().accept(*execMeStep);
/*			if(execMeStep->result_get()->isDouble())
			{
				pIL->step_set(((Double*)execMeStep->result_get())->real_get(0,0));
			}
			else if(execMeStep->result_get()->isPoly())
			{
				pIL->step_set(((Poly*)execMeStep->result_get()));
			}
*/

			e.end_get().accept(*execMeEnd);
/*			if(execMeEnd->result_get()->isDouble())
			{
				pIL->end_set(((Double*)execMeEnd->result_get())->real_get(0,0));
			}
			else if(execMeEnd->result_get()->isPoly())
			{
				pIL->end_set(((Poly*)execMeEnd->result_get()));
			}
*/

			ImplicitList *pIL	= new ImplicitList(
				execMeStart->result_get(),
				execMeStep->result_get(),
				execMeEnd->result_get());

			result_set(pIL);
			delete execMeStart;
			delete execMeStep;
			delete execMeEnd;
	}
	/** \} */

	InternalType	*ExecVisitor::result_get(void)
	{
		return _result;
	}

	void ExecVisitor::result_set(const InternalType *e)
	{
		_result = (InternalType *)e;
	}

}

bool bConditionState(ast::ExecVisitor *exec)
{
	if(((GenericType*)exec->result_get())->isDouble() && ((Double*)exec->result_get())->isComplex() == false)
	{
		Double *pR		= (Double*)exec->result_get();
		double *pReal	= pR->real_get();

		for(int i = 0 ; i < pR->size_get() ; i++)
		{
			if(pReal[i] == 0)
			{
				return false;
			}
		}
	}
	else if(((GenericType*)exec->result_get())->isBool())
	{
		Bool *pB		= (Bool*)exec->result_get();
		bool *pData		= pB->bool_get();

		for(int i = 0 ; i < pB->size_get() ; i++)
		{
			if(pData[i] == false)
			{
				return false;
				break;
			}
		}
	}
	else if(((GenericType*)exec->result_get())->isUInt())
	{
	}
	else if(((GenericType*)exec->result_get())->isInt())
	{
	}
	else
	{
		return false;

/*
		std::ostringstream os;
		os << "first argument is incorrect";
		os << " (" << e.test_get().location_get().first_line << "," << e.test_get().location_get().first_column << ")" << std::endl;
		string szErr(os.str());
		throw szErr;
*/
	}
	return true;
}

void vTransposeRealMatrix(double *_pdblRealIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut)
{
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
	{
		int iNewCoord	= iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);
		_pdblRealOut[iNewCoord]	= _pdblRealIn[iIndex];
	}
}

void vTransposeComplexMatrix(double *_pdblRealIn, double *_pdblImgIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut, double *_pdblImgOut, bool _bConjugate)
{
	int iIndex = 0;

	if(_bConjugate == false)
	{
		for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
		{
			int iNewCoord	= iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);

			_pdblRealOut[iNewCoord]	= _pdblRealIn[iIndex];
			_pdblImgOut[iNewCoord]	= _pdblImgIn[iIndex];
		}
	}
	else
	{
		for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
		{
			int iNewCoord	= iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);

			_pdblRealOut[iNewCoord]	= _pdblRealIn[iIndex];
			//Conjugate
			_pdblImgOut[iNewCoord]	= -_pdblImgIn[iIndex];
		}
	}
}
