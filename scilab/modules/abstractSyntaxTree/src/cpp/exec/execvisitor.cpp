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

#include "stack-def.h"

#include "execvisitor.hxx"
#include "shortcutvisitor.hxx"
#include "conditionvisitor.hxx"

#include "timer.hxx"
#include "localization.h"

#include "yaspio.hxx"

using std::string;

bool bConditionState(ast::ConditionVisitor *exec);

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
		InternalType *pI = symbol::Context::getInstance()->get(e.name_get());
		if(pI != NULL)
		{
			result_set(pI);
			if(pI != NULL && pI->getAsCallable() == false && e.is_verbose())
			{
			  std::ostringstream ostr;
			  ostr << e.name_get() << " = " << std::endl;
			  ostr << std::endl;
			  ostr << pI->toString(10,75) << std::endl;
			  YaspWrite((char *) ostr.str().c_str());
			}
		}
		else
		{
			char szError[bsiz];
#ifdef _MSC_VER
			sprintf_s(szError, bsiz, _("Undefined variable: %s\n"), e.name_get().c_str());
#else
			sprintf(szError, _("Undefined variable: %s\n"), e.name_get().c_str());
#endif
			throw string(szError);
			//Err, SimpleVar doesn't exist in Scilab scopes.
		}
	}

	void ExecVisitor::visit (const ColonVar &e)
	{
		int pRank[1] = {2};
		Double dblCoef(1,2);
		dblCoef.val_set(0, 0, 0);
		dblCoef.val_set(0, 1, 1);

		MatrixPoly *pVar	= new MatrixPoly("$", 1, 1, pRank);
		Poly *poPoly			= pVar->poly_get(0,0);
		poPoly->coef_set(&dblCoef);

		ImplicitList *pIL = new ImplicitList();
		pIL->start_set(new Double(1));
		pIL->step_set(new Double (1));
		pIL->end_set(pVar);
		result_set(pIL);
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
	  ExecVisitor execHead;
	 	try
		{
		  e.head_get()->accept(execHead);
		}
		catch (string sz) 
		  {
		    throw sz;
		  } 
		if (execHead.result_get() != NULL && !execHead.result_get()->isStruct())
		  {
		    char szError[bsiz];
#ifdef _MSC_VER
		    sprintf_s(szError, bsiz, _("Attempt to reference field of non-structure array.\n"));
#else
		    sprintf(szError, _("Attempt to reference field of non-structure array.\n"));
#endif
		    throw string(szError);
		  }
		else
		  {
		    // Manage 3 cases
		    // head.ID
		    // head.variable
		    // head.functionCall
		    SimpleVar *psvRightMember = dynamic_cast<SimpleVar *>(const_cast<Exp *>(e.tail_get()));
		    if (psvRightMember != NULL)
		      {
			Struct* psValue = execHead.result_get()->getAsStruct();
			if ( psValue->exists(psvRightMember->name_get()) )
			  {
			    result_set(psValue->get(psvRightMember->name_get())->clone());
			  }
			else 
			  {
			    char szError[bsiz];
#ifdef _MSC_VER
			    sprintf_s(szError, bsiz, _("Unknown field : %s.\n"), psvRightMember->name_get().c_str());
#else
			    sprintf(szError, _("Unknown field : %s.\n"), psvRightMember->name_get().c_str());
#endif
			    throw string(szError);
			  }
		      }
		    else
		      {
			char szError[bsiz];
#ifdef _MSC_VER
			sprintf_s(szError, bsiz, _("/!\\ Unmanaged FieldExp.\n"));
#else
			sprintf(szError, _("/!\\ Unmanaged FieldExp.\n"));
#endif
			throw string(szError);
		      }
		  }
	}

	void ExecVisitor::visit(const CallExp &e)
	{
		ExecVisitor execFunc;
		std::list<Exp *>::const_iterator	i;

		e.name_get().accept(execFunc);
		if(execFunc.result_get() != NULL && execFunc.result_get()->isCallable())
		{//function call
			Callable *pCall = execFunc.result_get()->getAsCallable();
			types::typed_list out;
			types::typed_list in;
			
			//find the good macro
			ExecVisitor *execVar = new ast::ExecVisitor[e.args_get().size()]();
			int j = 0;
			for (j = 0, i = e.args_get().begin (); i != e.args_get().end (); ++i,j++)
			{
				(*i)->accept (execVar[j]);
				if(execVar[j].result_get()->getType() == InternalType::RealImplicitList)
				{
					ImplicitList* pIL = execVar[j].result_get()->getAsImplicitList();
					execVar[j].result_set(pIL->extract_matrix());
					delete pIL;
				}
				
				if(execVar[j].is_single_result())
				{
						in.push_back(execVar[j].result_get());
						execVar[j].result_get()->IncreaseRef();
				}
				else
				{
					for(int i = 0 ; i < execVar[j].result_size_get() ; i++)
					{
						in.push_back(execVar[j].result_get(i));
						execVar[j].result_get(i)->IncreaseRef();
					}
				}
			}
			
			int iRetCount = Max(1, expected_size_get());
			Function::ReturnValue Ret = pCall->call(in, iRetCount, out);
			
			if(Ret == Callable::OK)
			{
				if(expected_size_get() == 1 && out.size() == 0) //to manage ans
				{
					if((int)out.size() < expected_size_get())
					{
						std::ostringstream os;
						os << "bad lhs, expected : " << expected_size_get() << " returned : " << out.size() << std::endl;
						throw os.str();
					}
				}

				for(int i = 0 ; i < out.size() ; i++)
				{
					result_set(i, out[i]);
				}
			}
			else if(Ret == Callable::Error)
			{
				std::ostringstream os;
				char szError[bsiz];
#ifdef _MSC_VER
				sprintf_s(szError, bsiz, _("Function \"%s\" failed\n"), pCall->getName().c_str());
#else
				sprintf(szError, _("Function \"%s\" failed\n"), pCall->getName().c_str());
#endif
				throw string(szError);
			}

			
			for (j = 0; j < e.args_get().size(); j++)
			{
				execVar[j].result_get()->DecreaseRef();
			}
			delete[] execVar;
		}
		else if(execFunc.result_get() != NULL)
		{//a(xxx) with a variable, extraction

			//get symbol of variable
			InternalType *pIT = NULL;

			//WARNING can be a fieldexp
			const SimpleVar *Var = dynamic_cast<const SimpleVar*>(&e.name_get());
			if(Var != NULL)
			{
				pIT = symbol::Context::getInstance()->get(Var->name_get());
			}
			else
			{
				pIT = execFunc.result_get();
			}
			InternalType *pOut			= NULL;
			std::vector<InternalType*> ResultList;
			int iArgDim							= (int)e.args_get().size();
			bool bSeeAsVector				= iArgDim == 1;

			//Create list of indexes
			//std::vector<std::vector<int>> IndexList;

			int *piIndexSeq		= NULL;
			int *piMaxDim			= NULL;
			int *piDimSize		= new int[iArgDim];
			int iTotalCombi		= GetIndexList(e.args_get(), &piIndexSeq, &piMaxDim, pIT, piDimSize);

			switch(pIT->getType())
			{
			case InternalType::RealDouble :
				pOut = pIT->getAsDouble()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
				break;
			case InternalType::RealBool : 
				pOut = pIT->getAsBool()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
				break;
			case InternalType::RealInt :
				pOut = pIT->getAsInt()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
				break;
			case InternalType::RealString :
				pOut = pIT->getAsString()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
				break;
			case InternalType::RealList :
				{
					ResultList = pIT->getAsList()->extract(iTotalCombi, piIndexSeq, piMaxDim, piDimSize, bSeeAsVector);
					for(int i = 0 ; i < ResultList.size() ; i++)
					{
						result_set(i, ResultList[i]);
					}
					break;
				}
			default :
				break;
			}

			//Container type can return multiple items
			if(pIT->isContainer() == false)
			{
				if(pOut == NULL)
				{
					std::ostringstream os;
					os << "inconsistent row/column dimensions";
					os << ((Location)(*e.args_get().begin())->location_get()).location_string_get() << std::endl;
					throw os.str();
				}
				result_set(pOut);
			}
			else 
			{
				if(ResultList.size() == 0)
				{
					std::ostringstream os;
					os << "inconsistent row/column dimensions";
					os << ((Location)(*e.args_get().begin())->location_get()).location_string_get() << std::endl;
					throw os.str();
				}
			}
		}
		else
		{//result == NULL ,variable doesn't exist :(
			std::ostringstream os;
			char pst[bsiz];
#ifdef _MSC_VER
			sprintf_s(pst, bsiz, _("Undefined variable %s.\n"), e.name_get());
#else
			sprintf(pst, _("Undefined variable %s.\n"), e.name_get());
#endif
			throw string(pst);
		}
	}

	void ExecVisitor::visit (const IfExp  &e)
	{
		//Create local exec visitor
		ConditionVisitor execMeTest;
		ShortCutVisitor SCTest;
		ExecVisitor execMeAction;
		bool bTestStatus							= false;

		//condition
		e.test_get().accept(SCTest);
		e.test_get().accept(execMeTest);

		bTestStatus = bConditionState(&execMeTest);
		if(bTestStatus == true)
		{//condition == true
			if(e.is_breakable())
			{
				((Exp*)&e.then_get())->breakable_set();
			}

			if(e.is_returnable())
			{
				((Exp*)&e.then_get())->returnable_set();
			}

			e.then_get().accept(execMeAction);
		}
		else
		{//condition == false

			if(e.has_else())
			{
				if(e.is_breakable())
				{
					((Exp*)&e.else_get())->breakable_set();
				}

				if(e.is_returnable())
				{
					((Exp*)&e.else_get())->returnable_set();
				}

				e.else_get().accept(execMeAction);
			}
		}

		if(e.is_breakable() && ( ((Exp*)&e.else_get())->is_break() || ((Exp*)&e.then_get())->is_break() ))
		{
			((Exp*)&e)->break_set();
			((Exp*)&e.else_get())->break_reset();
			((Exp*)&e.then_get())->break_reset();
		}

		if(e.is_returnable() && ( ((Exp*)&e.else_get())->is_return() || ((Exp*)&e.then_get())->is_return() ))
		{
			((Exp*)&e)->return_set();
			((Exp*)&e.else_get())->return_reset();
			((Exp*)&e.then_get())->return_reset();
		}
	}

	void ExecVisitor::visit (const TryCatchExp  &e)
	{
	}

	void ExecVisitor::visit (const WhileExp  &e)
	{
		ConditionVisitor execMeTest;
		ExecVisitor execMeAction;
		bool bTestStatus	= false;

		//allow break operation
		((Exp*)&e.body_get())->breakable_set();
		//allow return operation
		if(e.is_returnable())
		{
			((Exp*)&e.body_get())->is_returnable();
		}

		//condition
		e.test_get().accept(execMeTest);
		while(bConditionState(&execMeTest))
		{
			e.body_get().accept(execMeAction);
			if(e.body_get().is_break())
			{
				break;
			}

			if(e.body_get().is_return())
			{
				((Exp*)&e)->return_set();
				break;
			}
			e.test_get().accept(execMeTest);
		}
	}

	void ExecVisitor::visit (const ForExp  &e)
	{
		ExecVisitor execVar;
		e.vardec_get().accept(execVar);

		//allow break operation
		((Exp*)&e.body_get())->breakable_set();
		//allow return operation
		if(e.is_returnable())
		{
			((Exp*)&e.body_get())->is_returnable();
		}

		if(execVar.result_get()->getType() == InternalType::RealImplicitList)
		{
			ExecVisitor execBody;
			ImplicitList* pVar = (ImplicitList*)execVar.result_get();

			InternalType *pIT = NULL;
			pIT = pVar->extract_value(0);
			string varName = e.vardec_get().name_get();
			symbol::Context::getInstance()->put(varName, *pIT);

			for(int i = 0 ; i < pVar->size_get() ; i++)
			{
				if(pIT->getAsDouble())
				{
					Double *pDouble = pIT->getAsDouble();
					pDouble->real_get()[0] = pVar->extract_value_double(i);
				}
				else if(pIT->getAsInt())
				{
					Int *pInt = pIT->getAsInt();
					pInt->data_set(0, 0,  pVar->extract_value_int(i));
				}
				else
				{
					pIT = pVar->extract_value(i);
					symbol::Context::getInstance()->put(varName, *pIT);
				}

				e.body_get().accept(execBody);
				if(e.body_get().is_break())
				{
					break;
				}

				if(e.body_get().is_return())
				{
					((Exp*)&e)->return_set();
					break;
				}
			}
		}
		else
		{//Matrix i = [1,3,2,6] or other type
			ExecVisitor execBody;
			GenericType* pVar = (GenericType*)execVar.result_get();
			for(int i = 0 ; i < pVar->cols_get() ; i++)
			{
				GenericType* pNew = pVar->get_col_value(i);
				symbol::Context::getInstance()->put(e.vardec_get().name_get(), *pNew);
				e.body_get().accept(execBody);
				if(e.body_get().is_break())
				{
					break;
				}

				if(e.body_get().is_return())
				{
					((Exp*)&e)->return_set();
					break;
				}
			}
		}
	}

	void ExecVisitor::visit (const BreakExp &e)
	{
		((BreakExp*)&e)->break_set();
	}

	void ExecVisitor::visit (const ReturnExp &e)
	{
		if(e.is_global() == false)
		{//return(x)
			ExecVisitor execVar;
			e.exp_get().accept(execVar);
			
			for(int i = 0 ; i < execVar.result_size_get() ; i++)
			{
				result_set(i, execVar.result_get(i)->clone());
			}
		}
		((Exp*)&e)->return_set();
	}

	void ExecVisitor::visit (const SelectExp &e)
	{
	  // FIXME : exec select ... case ... else ... end
		ExecVisitor execMe;
		e.select_get()->accept(execMe);
		bool bCase = false;

		
		if(execMe.result_get() != NULL)
		{//find good case
			cases_t::iterator it;
			for(it = e.cases_get()->begin(); it != e.cases_get()->end() ; it++)
			{
				ExecVisitor execCase;
				CaseExp* pCase = *it;
				pCase->test_get()->accept(execCase);
				if(execCase.result_get() != NULL)
				{
					if(execCase.result_get()->isContainer()) //WARNING ONLY FOR CELL
					{//check each item
					}
					else if(*execCase.result_get() == *execMe.result_get())
					{//the good one
						ExecVisitor execBody;
						pCase->body_get()->accept(execBody);
						bCase = true;
					}
				}
			}
		}

		if(bCase == false)
		{//default case
			ExecVisitor execDefault;
			e.default_case_get()->accept(execDefault);
		}
	}

	void ExecVisitor::visit(const CaseExp &e)
	{
	  // FIXME : case ... 
	}


	void ExecVisitor::visit (const SeqExp  &e)
	{
		std::list<Exp *>::const_iterator	i;

		for (i = e.exps_get().begin (); i != e.exps_get().end (); ++i)
		{
			ExecVisitor execMe;
			if(e.is_breakable())
			{
				(*i)->breakable_set();
			}

			if(e.is_returnable())
			{
				(*i)->returnable_set();
			}

			(*i)->accept(execMe);

			if(execMe.result_get() != NULL)
			{
				cout << "result : " << execMe.result_get() << endl;
				if(execMe.result_get()->getAsCallable())//to manage call without ()
				{
					Callable *pCall = execMe.result_get()->getAsCallable();
					types::typed_list out;
					types::typed_list in;

					Function::ReturnValue Ret = pCall->call(in, (int)expected_size_get(), out);

					if(Ret == Callable::OK)
					{
						if(expected_size_get() == 1 && out.size() == 0) //to manage ans
						{
							if((int)out.size() < expected_size_get())
							{
								std::ostringstream os;
								os << "bad lhs, expected : " << expected_size_get() << " returned : " << out.size() << std::endl;
								throw os.str();
							}
						}

						for(int i = 0 ; i < out.size() ; i++)
						{
							out[i]->DecreaseRef();
							execMe.result_set(i, out[i]);
						}
					}
					else if(Ret == Callable::Error)
					{
						std::ostringstream os;
						char szError[bsiz];
#ifdef _MSC_VER
						sprintf_s(szError, bsiz, _("Function \"%s\" failed\n"), pCall->getName().c_str());
#else
						sprintf(szError, _("Function \"%s\" failed\n"), pCall->getName().c_str());
#endif
						throw string(szError);
					}
				}

				if(execMe.result_get()->isDeletable())
				{
					symbol::Context::getInstance()->put("ans", *execMe.result_get());
					if((*i)->is_verbose())
					{
						//TODO manage multiple returns
						std::ostringstream ostr;
						ostr << "ans = " << std::endl;
						ostr << std::endl;
						ostr << execMe.result_get()->toString(10,75) << std::endl;
						YaspWrite((char *)ostr.str().c_str());
					}
				}

			}

			if(((SeqExp*)&e)->is_breakable() && (*i)->is_break())
			{
				((SeqExp*)&e)->break_set();
				break;
			}

			if(((SeqExp*)&e)->is_returnable() && (*i)->is_return())
			{
				((SeqExp*)&e)->return_set();
				((SeqExp*)(*i))->return_reset();
				break;
			}
		}
	}

	void ExecVisitor::visit (const ArrayListExp  &e)
	{
		std::list<Exp *>::const_iterator it;
		int i = 0;
		for(it = e.exps_get().begin() ; it != e.exps_get().end() ; it++)
		{
			ExecVisitor execArg;
			(*it)->accept(execArg);
			result_set(i, execArg.result_get()->clone());
			i++;
		}
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
		ExecVisitor execMe;
		e.exp_get().accept(execMe);

		if(execMe.result_get()->isDouble())
		{
			Double *pdbl	= execMe.result_get()->getAsDouble();
			Bool *pReturn	= new Bool(pdbl->rows_get(), pdbl->cols_get());
			double *pR		= pdbl->real_get();
			int *piB			= pReturn->bool_get();
			for(int i = 0 ; i < pdbl->size_get() ; i++)
			{
				piB[i] = pR[i] == 0 ? 1 : 0;
			}
			result_set(pReturn);
		}
		else if(execMe.result_get()->isBool())
		{
			Bool *pb			= execMe.result_get()->getAsBool();
			Bool *pReturn	= new Bool(pb->rows_get(), pb->cols_get());
			int *piR			= pb->bool_get();
			int *piB			= pReturn->bool_get();

			for(int i = 0 ; i < pb->size_get() ; i++)
			{
				piB[i] = piR[i] == 1 ? 0 : 1;
			}
			result_set(pReturn);
		}
	}

	void ExecVisitor::visit (const TransposeExp &e)
	{
		/*
		'
		*/
		ExecVisitor execMe;
		e.exp_get().accept(execMe);

		bool bConjug = e.conjugate_get() == TransposeExp::_Conjugate_;

		if(execMe.result_get()->isImplicitList())
		{
			InternalType *pIT = execMe.result_get()->getAsImplicitList()->extract_matrix();
			execMe.result_set(pIT);
		}

		if(execMe.result_get()->isDouble())
		{
			Double *pdbl		= execMe.result_get()->getAsDouble();
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
		else if(execMe.result_get()->isPoly())
		{
			MatrixPoly *pMP			= execMe.result_get()->getAsPoly();
			MatrixPoly *pReturn	= NULL;

			//prepare rank array
			int* piRank = new int[pMP->size_get()];

			for(int i = 0 ; i < pMP->rows_get() ; i++)
			{
				for(int j = 0 ; j < pMP->cols_get() ; j++)
				{
					piRank[i * pMP->cols_get() + j] = pMP->poly_get(i,j)->rank_get();
				}
			}

			pReturn = new MatrixPoly(pMP->var_get(), pMP->cols_get(), pMP->rows_get(), piRank);
			pReturn->complex_set(pMP->isComplex());

			if(pMP->isComplex() && bConjug)
			{
				for(int i = 0 ; i < pMP->rows_get() ; i++)
				{
					for(int j = 0 ; j < pMP->cols_get() ; j++)
					{
						pReturn->poly_set(j, i, pMP->poly_get(i,j)->coef_get());
						double *pdblImg = pReturn->poly_get(j, i)->coef_img_get();
						for(int k = 0 ; k < pReturn->poly_get(j, i)->rank_get() ; k++)
						{
							pdblImg[k] *= -1;
						}
					}
				}
			}
			else
			{
				for(int i = 0 ; i < pMP->rows_get() ; i++)
				{
					for(int j = 0 ; j < pMP->cols_get() ; j++)
					{
						pReturn->poly_set(j, i, pMP->poly_get(i,j)->coef_get());
					}
				}
			}

			result_set(pReturn);
		}
}
	/** \} */

	/** \name Visit Declaration nodes.
	** \{ */
	/** \brief Visit Var declarations. */
	void ExecVisitor::visit (const VarDec  &e)
	{
		/*Create local exec visitor*/
		ExecVisitor execMe;
		try
		{
			/*getting what to assign*/
			e.init_get().accept(execMe);
			result_set(execMe.result_get());
			result_get()->IncreaseRef();
		}
		catch(string sz)
		{
			throw sz;
		}
	}

	void ExecVisitor::visit (const FunctionDec  &e)
	{
		/*
		function foo
		endfunction
		*/
		std::list<ast::Var *>::const_iterator	i;

		//get input parameters list
		std::list<string> *pVarList = new std::list<string>();
		ArrayListVar *pListVar = (ArrayListVar *)&e.args_get();
		for(i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
		{
			pVarList->push_back(((SimpleVar*)(*i))->name_get());
		}

		//get output parameters list
		std::list<string> *pRetList = new std::list<string>();
		ArrayListVar *pListRet = (ArrayListVar *)&e.returns_get();
		for(i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
		{
			pRetList->push_back(((SimpleVar*)(*i))->name_get());
		}

		//types::Macro macro(VarList, RetList, (SeqExp&)e.body_get());
		types::Macro *pMacro = new types::Macro(e.name_get(), *pVarList, *pRetList, (SeqExp&)e.body_get(), "script");
		symbol::Context::getInstance()->AddMacro(pMacro);
	}
	/** \} */

	/** \name Visit Type dedicated Expressions related node.
	** \{ */
	void ExecVisitor::visit(const ListExp &e)
	{
		ExecVisitor	execMeStart;
		ExecVisitor	execMeStep;
		ExecVisitor	execMeEnd;

		try
		{
			e.start_get().accept(execMeStart);
			execMeStart.result_get()->IncreaseRef();
			GenericType* pITStart = (GenericType*)execMeStart.result_get();
			if(pITStart->rows_get() != 1 || pITStart->cols_get() != 1)
			{
				throw 1;
			}


			e.step_get().accept(execMeStep);
			execMeStep.result_get()->IncreaseRef();
			GenericType* pITStep = (GenericType*)execMeStep.result_get();
			if(pITStep->rows_get() != 1 || pITStep->cols_get() != 1)
			{
				throw 2;
			}

			e.end_get().accept(execMeEnd);
			execMeEnd.result_get()->IncreaseRef();
			GenericType* pITEnd = (GenericType*)execMeEnd.result_get();
			if(pITEnd->rows_get() != 1 || pITEnd->cols_get() != 1)
			{
				throw 3;
			}

			//check compatibility

			if(execMeStart.result_get()->getType() == InternalType::RealInt)
			{//if Step or End are Int too, they must have the same precision
				Int::IntType IT = execMeStart.result_get()->getAsInt()->getIntType();

				if(execMeStep.result_get()->getType() == InternalType::RealInt)
				{
					if(execMeStep.result_get()->getAsInt()->getIntType() != IT)
					{
						throw string(_("Undefined operation for the given operands.\n"));
					}
				}
				else if(execMeStep.result_get()->getType() == InternalType::RealPoly)
				{
					throw string(_("Undefined operation for the given operands.\n"));
				}


				if(execMeEnd.result_get()->getType() == InternalType::RealInt)
				{
					if(execMeEnd.result_get()->getAsInt()->getIntType() != IT)
					{
						throw string(_("Undefined operation for the given operands.\n"));
					}
				}
				else if(execMeEnd.result_get()->getType() == InternalType::RealPoly)
				{
					throw string(_("Undefined operation for the given operands.\n"));
				}
			}
			else if(execMeStart.result_get()->getType() == InternalType::RealPoly)
			{
				if(execMeStep.result_get()->getType() == InternalType::RealInt)
				{
					throw string(_("Undefined operation for the given operands.\n"));
				}

				if(execMeEnd.result_get()->getType() == InternalType::RealInt)
				{
					throw string(_("Undefined operation for the given operands.\n"));
				}
			}
			else if(execMeStep.result_get()->getType() == InternalType::RealInt)
			{//if End are Int too, they must have the same precision
				Int::IntType IT = execMeStep.result_get()->getAsInt()->getIntType();

				if(execMeEnd.result_get()->getType() == InternalType::RealInt)
				{
					if(execMeEnd.result_get()->getAsInt()->getIntType() != IT)
					{
						throw string(_("Undefined operation for the given operands.\n"));
					}
				}
			}
			else if(execMeStep.result_get()->getType() == InternalType::RealPoly)
			{
				if(execMeEnd.result_get()->getType() == InternalType::RealInt)
				{
					throw string(_("Undefined operation for the given operands.\n"));
				}
			}

			ImplicitList *pIL	= new ImplicitList(
				execMeStart.result_get(),
				execMeStep.result_get(),
				execMeEnd.result_get());

			result_set(pIL);
		}
		catch(int iPos)
		{
			char st[bsiz];
#ifdef _MSC_VER
			sprintf_s(st, bsiz, _("%s: Wrong type for argument %d: Scalar expected.\n"), ":", iPos);
#else
			sprintf(st, _("%s: Wrong type for argument %d: Scalar expected.\n"), "::", 1);
#endif
			throw string(st);
		}
		catch(string sz)
		{		
			//TODO YaSp : Overloading
			throw sz;
		}
	}
	/** \} */

	int ExecVisitor::expected_size_get(void)
	{
		return _excepted_result;
	}

	int ExecVisitor::result_size_get(void)
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
			return (int)_resultVect.size();
		}
	}

	void ExecVisitor::expected_size_set(int _iSize)
	{
		_excepted_result = _iSize;
	}

	InternalType* ExecVisitor::result_get(void)
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

	types::InternalType* ExecVisitor::result_get(int _iPos)
	{
		if(_iPos >= _resultVect.size())
		{
			return NULL;
		}
		return _resultVect[_iPos];
	}

	vector<types::InternalType*>* ExecVisitor::result_list_get()
	{
		return &_resultVect;
	}

	void ExecVisitor::result_set(int _iPos, const types::InternalType *gtVal)
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

		_resultVect[_iPos] = (InternalType *)gtVal;
	}

	void ExecVisitor::result_set(const InternalType *gtVal)
	{
		m_bSingleResult = true;
		_result = const_cast<InternalType *>(gtVal);
	}

	bool ExecVisitor::is_single_result()
	{
		return m_bSingleResult;
	}
			
}

using namespace ast;
//using ast::ExecVisitor;

bool bConditionState(ast::ConditionVisitor *exec)
{
	if(exec->is_boolean_result())
	{
		return exec->result_bool_get();
	}
	else if(((GenericType*)exec->result_get())->isDouble() && ((Double*)exec->result_get())->isComplex() == false)
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
		int *piData	= pB->bool_get();

		for(int i = 0 ; i < pB->size_get() ; i++)
		{
			if(piData[i] == 0)
			{
				return false;
				break;
			}
		}
	}
	else if(((GenericType*)exec->result_get())->isInt())
	{
	}
	else
	{
		return false;
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


int GetIndexList(std::list<ast::Exp *> _plstArg, int** _piIndexSeq, int** _piMaxDim, InternalType *_pRefVar, int *_iDimSize)
{
	//Create list of indexes
	//std::vector<std::vector<int>> IndexList;
	symbol::Context *pcontext = symbol::Context::getInstance();
	int iProductElem				= (int)_plstArg.size();
	int **piIndexList				= NULL;
	int *piTabsize					= NULL;
	int iTotalCombi					= 1;
	int k										= 0;

	piTabsize			= new int[iProductElem];
	(*_piMaxDim)	= new int[iProductElem];
	piIndexList		= new int*[iProductElem];

	ExecVisitor execMeArg;
	std::list<Exp *>::const_iterator	i;
	for(i = _plstArg.begin() ; i != _plstArg.end() ; i++,k++)
	{
		(*i)->accept(execMeArg);
		InternalType *pIn = NULL;
		Double *pDbl = NULL;
		bool bDeleteDbl = false;

		if(execMeArg.result_get()->getType() == InternalType::RealImplicitList)
		{//a:b:c
			int iMaxDim = 0;
			if(_pRefVar != NULL)
			{
				iMaxDim = GetVarMaxDim(_pRefVar, k, iProductElem);
			}

			Double dbl(iMaxDim); // $
			ImplicitList *pIL = execMeArg.result_get()->getAsImplicitList();
			if(pIL->computable() == false)
			{
				if(pIL->start_type_get() == InternalType::RealPoly)
				{
					MatrixPoly *poPoly	= (MatrixPoly*)pIL->start_get();
					pIL->start_set(poPoly->evaluate(&dbl));
				}
				if(pIL->step_type_get() == InternalType::RealPoly)
				{
					MatrixPoly *poPoly	= (MatrixPoly*)pIL->step_get();
					pIL->step_set(poPoly->evaluate(&dbl));
				}
				if(pIL->end_type_get() == InternalType::RealPoly)
				{
					MatrixPoly *poPoly	= (MatrixPoly*)pIL->end_get();
					pIL->end_set(poPoly->evaluate(&dbl));
				}
			}

			pDbl = (Double*)pIL->extract_matrix();
			bDeleteDbl = true;
		}
		else if(execMeArg.result_get()->getType() == InternalType::RealBool)
		{
			Bool *pB			= execMeArg.result_get()->getAsBool();
			int *piB			= pB->bool_get();

			//find true item count
			int iItemCount = 0;
			for(int i = 0 ; i < pB->size_get() ; i++)
			{
				if(piB[i])
				{
					iItemCount++;
				}
			}

			//allow new Double variable
			pDbl					= new Double(iItemCount, 1);
			double* pdbl	= pDbl->real_get();

			int j = 0;
			for(int i = 0 ; i < pB->size_get() ; i++)
			{
				if(piB[i])
				{
					pdbl[j++] = i + 1;
				}
			}
			
			bDeleteDbl		= true;
		}
		else
		{
			pIn = execMeArg.result_get();

			if(pIn->getType() == InternalType::RealPoly)
			{//manage $
				MatrixPoly *pPoly = pIn->getAsPoly();

				if(_pRefVar != NULL)
				{
					int iMaxDim = GetVarMaxDim(_pRefVar, k, iProductElem);
					Double dbl(iMaxDim); // $
					pDbl = pPoly->evaluate(&dbl);
					bDeleteDbl = true;
				}
				else
				{//houston we have a problem ...
					Double dbl(0);
					pDbl = pPoly->evaluate(&dbl);

				}
			}
			else if(pIn->getType() == InternalType::RealDouble)
			{
				pDbl	= pIn->getAsDouble();
			}
			else
			{//Heu ... ?
			}
		}

		double *pData = pDbl->real_get();

		piTabsize[k] = pDbl->size_get();
		piIndexList[k] = new int[piTabsize[k]];

		(*_piMaxDim)[k] = (int)(pData[0] + 0.5);
		int iSize = pDbl->size_get();
		if(_iDimSize != NULL)
		{
			_iDimSize[k] = iSize;
		}

		for(int j = 0 ; j < iSize ; j++)
		{
			if((int)(pData[j] + 0.5) > (*_piMaxDim)[k])
			{
				(*_piMaxDim)[k] = (int)(pData[j] + 0.5);
			}
			//SubList.push_back((int)pData[j]);
			piIndexList[k][j] = (int)(pData[j] + 0.5);

		}
		iTotalCombi *= iSize;
		//IndexList.push_back(SubList);

		if(bDeleteDbl == true)
		{
			delete pDbl;
		}
	}

	int iTabsize	= iTotalCombi * iProductElem;
	*_piIndexSeq	= new int[iTabsize];
	ExpandList(piIndexList, piTabsize, iProductElem, *_piIndexSeq);
	return iTotalCombi;
}

void ExpandList(int ** _piList, int *_piListSize, int _iListSizeSize, int *_piResultList)
{
	for(int i = _iListSizeSize - 1 ; i >= 0 ; i--)
	{
		int iPreOcc = 1;
		int iPostOcc = 1;
		for(int k = 0 ; k < i ; k++)
		{
			if(k != i)
			{
				iPreOcc *= _piListSize[k];
			}
		}

		for(int k = i + 1 ; k < _iListSizeSize ; k++)
		{
			if(k != i)
			{
				iPostOcc *= _piListSize[k];
			}
		}

		int iSize = _piListSize[i];
		for(int m = iSize - 1 ; m >= 0  ; m--)
		{
			for(int j1 = iPreOcc - 1 ; j1 >= 0 ; j1--)
			{
				for(int j2 = iPostOcc - 1 ; j2 >= 0 ; j2--)
				{
					//Bloc Offset				-> j1 * ( iPostOcc * iSize) * _iListSizeSize
					//Offset in Bloc		-> m * iPostOcc * _iListSizeSize
					//Offset in SubBloc -> j2 * _iListSizeSize + i
					int iPos = j1 * ( iPostOcc * iSize) * _iListSizeSize + m * iPostOcc * _iListSizeSize + j2 * _iListSizeSize + i;
					_piResultList[iPos] = _piList[i][m];
				}
			}
		}
	}
}

int GetVarMaxDim(InternalType *_pIT, int _iCurrentDim, int _iMaxDim)
{
	if(_pIT == NULL)
	{
		return 0;
	}
	if(_iCurrentDim == 0)
	{
		if(_iMaxDim != 1)
		{
			return ((GenericType*)_pIT)->rows_get();
		}
		else
		{
			return ((GenericType*)_pIT)->size_get();
		}
	}
	else if(_iCurrentDim == 1)
	{
		return ((GenericType*)_pIT)->cols_get();
	}
	else
	{//more than 2 dimansions ? :(
		return ((GenericType*)_pIT)->size_get();
	}
}
