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
#include <iostream>

#include "visitor_common.hxx"
#include "runvisitor.hxx"
#include "execvisitor.hxx"
#include "timedvisitor.hxx"
#include "shortcutvisitor.hxx"
#include "conditionvisitor.hxx"

extern "C"
{
#include "matrix_division.h"
}

#include "timer.hxx"
#include "localization.h"

#include "yaspio.hxx"
#include "context.hxx"

using std::string;

namespace ast
{
    template <class T>
    void RunVisitorT<T>::expected_size_set(int _iSize)
    {
        _excepted_result = _iSize;
    }
    
    template <class T>
    int RunVisitorT<T>::expected_size_get(void)
    {
        return _excepted_result;
    }

    template <class T>
    int RunVisitorT<T>::result_size_get(void)
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

    template <class T>
    types::InternalType* RunVisitorT<T>::result_get(void)
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

    template <class T>
    types::InternalType* RunVisitorT<T>::result_get(int _iPos)
    {
        if(_iPos >= static_cast<int>(_resultVect.size()))
        {
            return NULL;
        }
        return _resultVect[_iPos];
    }

    template <class T>
    vector<types::InternalType*>* RunVisitorT<T>::result_list_get()
    {
        return &_resultVect;
    }


    template <class T>
    void RunVisitorT<T>::result_set(int _iPos, const types::InternalType *gtVal)
    {
        m_bSingleResult = false;
        if(_iPos <  static_cast<int>(_resultVect.size()))
        {
            if(_resultVect[_iPos] != NULL && _resultVect[_iPos]->isDeletable())
            {
                delete _resultVect[_iPos];
            }
        }

        if(_iPos >=  static_cast<int>(_resultVect.size()))
        {
            _resultVect.resize(_iPos + 1, NULL);
        }

        _resultVect[_iPos] = const_cast<types::InternalType *>(gtVal);
    }

    template <class T>
    void RunVisitorT<T>::result_set(const types::InternalType *gtVal)
    {
        m_bSingleResult = true;
        _result = const_cast<types::InternalType *>(gtVal);
    }

    template <class T>
    bool RunVisitorT<T>::is_single_result()
    {
        return m_bSingleResult;
    }




	template <class T>
	void RunVisitorT<T>::visitprivate(const MatrixLineExp &e)
	{
        /*
          All processes are done in MatrixExp
        */
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const CellExp &e)
	{
		/*
          FIXME : container type
		*/
	}

	/** \name Visit Constant Expressions nodes.
     ** \{ */
	template <class T>
	void RunVisitorT<T>::visitprivate(const StringExp &e)
	{
		String *psz = new String(e.value_get().c_str());
		result_set(psz);
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const CommentExp &e)
	{
		/*
          Nothing to do
		*/
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const IntExp  &e)
	{
		/*
          Int does not exist, Int8 - 16 - 32 - 64 functions
		*/
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const FloatExp  &e)
	{
		/*
          Float does not exist, float function
		*/
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const DoubleExp  &e)
	{
		Double *pdbl = new Double(e.value_get());
		result_set(pdbl);
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const BoolExp  &e)
	{
		Bool *pb = new Bool(e.value_get());
		result_set(pb);
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const NilExp &e)
	{
		/*
          FIXME :
		*/
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const SimpleVar &e)
	{
		InternalType *pI = symbol::Context::getInstance()->get(e.name_get());
		if(pI != NULL)
		{
			result_set(pI);
			if(pI != NULL && pI->getAsCallable() == false && e.is_verbose())
			{
                std::ostringstream ostr;
				ostr << e.name_get() << " = " << "(" << pI->getRef() << ")"<< std::endl;
                ostr << std::endl;
                ostr << pI->toString(10,75) << std::endl;
                YaspWrite(const_cast<char *>(ostr.str().c_str()));
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

	template <class T>
	void RunVisitorT<T>::visitprivate(const ColonVar &e)
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
		pIL->step_set(new Double(1));
		pIL->end_set(pVar);
		result_set(pIL);
		/*
          : = 1:$
		*/
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const DollarVar &e)
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

	template <class T>
	void RunVisitorT<T>::visitprivate(const ArrayListVar &e)
	{
		/*

         */
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const FieldExp &e)
	{
        /*
          a.b
        */
        T execHead;
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
                    InternalType* pIT = psValue->get(psvRightMember->name_get());
                    result_set(pIT->clone());
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

	template <class T>
	void RunVisitorT<T>::visitprivate(const CallExp &e)
	{
		T execFunc;
		std::list<Exp *>::const_iterator	itExp;

		e.name_get().accept(execFunc);
		if(execFunc.result_get() != NULL && execFunc.result_get()->isCallable())
		{//function call
			Callable *pCall = execFunc.result_get()->getAsCallable();
			types::typed_list out;
			types::typed_list in;

			//get function arguments
			T *execVar = new T[e.args_get().size()]();
			int j = 0;
			for (j = 0, itExp = e.args_get().begin (); itExp != e.args_get().end (); ++itExp,j++)
			{
				(*itExp)->accept (execVar[j]);
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

			ExecVisitor execCall;
			Function::ReturnValue Ret = pCall->call(in, iRetCount, out, &execCall);
			
			if(Ret == Callable::OK)
			{
				if(expected_size_get() == 1 && out.size() == 0) //to manage ans
				{
					if(static_cast<int>(out.size()) < expected_size_get())
					{
						std::ostringstream os;
						os << "bad lhs, expected : " << expected_size_get() << " returned : " << out.size() << std::endl;
						throw os.str();
					}
				}

				if(out.size() == 1)
				{
					result_set(out[0]);
				}
				else 
				{
					for(int i = 0 ; i < static_cast<int>(out.size()) ; i++)
					{
						result_set(i, out[i]);
					}
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

			
			for (unsigned int k = 0; k < e.args_get().size(); k++)
			{
				execVar[k].result_get()->DecreaseRef();
			}
			
//			std::cout << "before delete[]" << std::endl;
			delete[] execVar;
//			std::cout << "after delete[]" << std::endl;
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
			int iArgDim							= static_cast<int>(e.args_get().size());
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
                for(int i = 0 ; i < static_cast<int>(ResultList.size()) ; i++)
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
					os << ((*e.args_get().begin())->location_get()).location_string_get() << std::endl;
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
					os << ((*e.args_get().begin())->location_get()).location_string_get() << std::endl;
					throw os.str();
				}
			}
			
			delete[] piDimSize;
			delete[] piIndexSeq;
			delete[] piMaxDim;
		}
		else
		{
            //result == NULL ,variable doesn't exist :(
            // Sould never be in this case
            // In worst case variable pointing to function does not exists
            // visitprivate(SimpleVar) will throw the right exception.
		}
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const IfExp  &e)
	{
		//Create local exec visitor
		ConditionVisitor execMeTest;
		ShortCutVisitor SCTest;
		T execMeAction;
		bool bTestStatus							= false;

		//condition
		e.test_get().accept(SCTest);
		e.test_get().accept(execMeTest);

		bTestStatus = bConditionState(&execMeTest);
		if(bTestStatus == true)
		{//condition == true
			if(e.is_breakable())
			{
				const_cast<Exp*>(&e.then_get())->breakable_set();
			}

			if(e.is_returnable())
			{
				const_cast<Exp*>(&e.then_get())->returnable_set();
			}

			e.then_get().accept(execMeAction);
		}
		else
		{//condition == false

			if(e.has_else())
			{
				if(e.is_breakable())
				{
					const_cast<Exp*>(&e.else_get())->breakable_set();
				}

				if(e.is_returnable())
				{
					const_cast<Exp*>(&e.else_get())->returnable_set();
				}

				e.else_get().accept(execMeAction);
			}
		}

		if(e.is_breakable() 
           && ( (&e.else_get())->is_break() 
                || (&e.then_get())->is_break() ))
		{
			const_cast<IfExp*>(&e)->break_set();
			const_cast<Exp*>(&e.else_get())->break_reset();
			const_cast<Exp*>(&e.then_get())->break_reset();
		}

		if(e.is_returnable() 
           && ( (&e.else_get())->is_return() 
                || (&e.then_get())->is_return() ))
		{
			const_cast<IfExp*>(&e)->return_set();
			const_cast<Exp*>(&e.else_get())->return_reset();
			const_cast<Exp*>(&e.then_get())->return_reset();
		}
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const TryCatchExp  &e)
	{
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const WhileExp  &e)
	{
		ConditionVisitor execMeTest;
		T execMeAction;

		//allow break operation
		const_cast<Exp*>(&e.body_get())->breakable_set();
		//allow return operation
		if(e.is_returnable())
		{
			(&e.body_get())->is_returnable();
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
				const_cast<WhileExp*>(&e)->return_set();
				break;
			}
			e.test_get().accept(execMeTest);
		}
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const ForExp  &e)
	{
		T execVar;
		e.vardec_get().accept(execVar);

		//allow break operation
		const_cast<Exp*>(&e.body_get())->breakable_set();
		//allow return operation
		if(e.is_returnable())
		{
			(&e.body_get())->is_returnable();
		}

		if(execVar.result_get()->getType() == InternalType::RealImplicitList)
		{
			T execBody;
			ImplicitList* pVar = execVar.result_get()->getAsImplicitList();
//			std::cout << "ImplicitList references : " << pVar->getRef() << std::endl;

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
					const_cast<ForExp*>(&e)->return_set();
					break;
				}
			}
			
			pVar->DecreaseRef();
		}
		else
		{//Matrix i = [1,3,2,6] or other type
			T execBody;
			GenericType* pVar = static_cast<GenericType*>(execVar.result_get());
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
					const_cast<ForExp*>(&e)->return_set();
					break;
				}
			}
		}
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const BreakExp &e)
	{
		const_cast<BreakExp*>(&e)->break_set();
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const ReturnExp &e)
	{
		if(e.is_global() == false)
		{//return(x)
			T execVar;
			e.exp_get().accept(execVar);
			
			for(int i = 0 ; i < execVar.result_size_get() ; i++)
			{
				result_set(i, execVar.result_get(i)->clone());
			}
		}
		const_cast<ReturnExp*>(&e)->return_set();
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const SelectExp &e)
	{
        // FIXME : exec select ... case ... else ... end
		T execMe;
		e.select_get()->accept(execMe);
		bool bCase = false;

		
		if(execMe.result_get() != NULL)
		{//find good case
			cases_t::iterator it;
			for(it = e.cases_get()->begin(); it != e.cases_get()->end() ; it++)
			{
				T execCase;
				CaseExp* pCase = *it;
				pCase->test_get()->accept(execCase);
				if(execCase.result_get() != NULL)
				{
					if(execCase.result_get()->isContainer()) //WARNING ONLY FOR CELL
					{//check each item
					}
					else if(*execCase.result_get() == *execMe.result_get())
					{//the good one
						T execBody;
						pCase->body_get()->accept(execBody);
						bCase = true;
					}
				}
			}
		}

		if(bCase == false)
		{//default case
			T execDefault;
			e.default_case_get()->accept(execDefault);
		}
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const CaseExp &e)
	{
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const SeqExp  &e)
	{
		std::list<Exp *>::const_iterator	itExp;

		for (itExp = e.exps_get().begin (); itExp != e.exps_get().end (); ++itExp)
		{
			T execMe;
			if(e.is_breakable())
			{
				(*itExp)->breakable_set();
			}

			if(e.is_returnable())
			{
				(*itExp)->returnable_set();
			}

			(*itExp)->accept(execMe);

			if(execMe.result_get() != NULL)
			{
				if(execMe.result_get()->getAsCallable())//to manage call without ()
				{
					Callable *pCall = execMe.result_get()->getAsCallable();
					types::typed_list out;
					types::typed_list in;

					T execCall;
					Function::ReturnValue Ret = pCall->call(in, expected_size_get(), out, &execCall);

					if(Ret == Callable::OK)
					{
						if(expected_size_get() == 1 && out.size() == 0) //to manage ans
						{
							if(static_cast<int>(out.size()) < expected_size_get())
							{
								std::ostringstream os;
								os << "bad lhs, expected : " << expected_size_get() << " returned : " << out.size() << std::endl;
								throw os.str();
							}
						}

						for(int i = 0 ; i < static_cast<int>(out.size()) ; i++)
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
					if((*itExp)->is_verbose())
					{
						//TODO manage multiple returns
						std::ostringstream ostr;
						ostr << "ans = " << std::endl;
						ostr << std::endl;
						ostr << execMe.result_get()->toString(10,75) << std::endl;
						YaspWrite(const_cast<char *>(ostr.str().c_str()));
					}
				}

			}

			if((&e)->is_breakable() && (*itExp)->is_break())
			{
				const_cast<SeqExp *>(&e)->break_set();
				break;
			}

			if((&e)->is_returnable() && (*itExp)->is_return())
			{
				const_cast<SeqExp *>(&e)->return_set();
				(*itExp)->return_reset();
				break;
			}
		}
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const ArrayListExp  &e)
	{
		std::list<Exp *>::const_iterator it;
		int i = 0;
		for(it = e.exps_get().begin() ; it != e.exps_get().end() ; it++)
		{
			T execArg;
			(*it)->accept(execArg);
			result_set(i, execArg.result_get()->clone());
			i++;
		}
	}

	template <class T>
	void RunVisitorT<T>::visitprivate(const AssignListExp  &e)
	{
	}
	/** \} */

	/** \name Visit Single Operation nodes.
     ** \{ */
	template <class T>
	void RunVisitorT<T>::visitprivate(const NotExp &e)
	{
		/*
          @ or ~= !
		*/
		T execMe;
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

	template <class T>
	void RunVisitorT<T>::visitprivate(const TransposeExp &e)
	{
		/*
          '
		*/
		T execMe;
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
	template <class T>
	void RunVisitorT<T>::visitprivate(const VarDec  &e)
	{
		/*Create local exec visitor*/
		T execMe;
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

	template <class T>
	void RunVisitorT<T>::visitprivate(const FunctionDec  &e)
	{
		/*
          function foo
          endfunction
		*/
		std::list<ast::Var *>::const_iterator	i;

		//get input parameters list
		std::list<string> *pVarList = new std::list<string>();
		const ArrayListVar *pListVar = &e.args_get();
		for(i = pListVar->vars_get().begin() ; i != pListVar->vars_get().end() ; i++)
		{
			pVarList->push_back(static_cast<SimpleVar*>(*i)->name_get());
		}

		//get output parameters list
		std::list<string> *pRetList = new std::list<string>();
		const ArrayListVar *pListRet = &e.returns_get();
		for(i = pListRet->vars_get().begin() ; i != pListRet->vars_get().end() ; i++)
		{
			pRetList->push_back(static_cast<SimpleVar*>(*i)->name_get());
		}

		//types::Macro macro(VarList, RetList, (SeqExp&)e.body_get());
		types::Macro *pMacro = new types::Macro(e.name_get(), *pVarList, *pRetList, 
                                                static_cast<SeqExp&>(const_cast<Exp&>(e.body_get())), "script");
		symbol::Context::getInstance()->AddMacro(pMacro);
	}
	/** \} */

	/** \name Visit Type dedicated Expressions related node.
     ** \{ */
	template <class T>
	void RunVisitorT<T>::visitprivate(const ListExp &e)
	{
		T	execMeStart;
		T	execMeStep;
		T	execMeEnd;

		try
		{
			e.start_get().accept(execMeStart);
			GenericType* pITStart = static_cast<GenericType*>(execMeStart.result_get());
			if(pITStart->rows_get() != 1 || pITStart->cols_get() != 1)
			{
				throw 1;
			}


			e.step_get().accept(execMeStep);
			GenericType* pITStep = static_cast<GenericType*>(execMeStep.result_get());
			if(pITStep->rows_get() != 1 || pITStep->cols_get() != 1)
			{
				throw 2;
			}

			e.end_get().accept(execMeEnd);
			GenericType* pITEnd = static_cast<GenericType*>(execMeEnd.result_get());
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

	template <class T>
	int RunVisitorT<T>::GetIndexList(std::list<ast::Exp *>const& _plstArg, int** _piIndexSeq, int** _piMaxDim, InternalType *_pRefVar, int *_iDimSize)
	{
		//Create list of indexes
		//std::vector<std::vector<int>> IndexList;
		int iProductElem				= _plstArg.size();
		int **piIndexList				= NULL;
		int *piTabsize					= NULL;
		int iTotalCombi					= 1;
		int k										= 0;

		piTabsize			= new int[iProductElem];
		piIndexList		= new int*[iProductElem];

		(*_piMaxDim)	= new int[iProductElem];

		T execMeArg;
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
						MatrixPoly *poPoly	= pIL->start_get()->getAsPoly();
						pIL->start_set(poPoly->evaluate(&dbl));
					}
					if(pIL->step_type_get() == InternalType::RealPoly)
					{
						MatrixPoly *poPoly	= pIL->step_get()->getAsPoly();
						pIL->step_set(poPoly->evaluate(&dbl));
					}
					if(pIL->end_type_get() == InternalType::RealPoly)
					{
						MatrixPoly *poPoly	= pIL->end_get()->getAsPoly();
						pIL->end_set(poPoly->evaluate(&dbl));
					}
				}

				pDbl = pIL->extract_matrix()->getAsDouble();
				bDeleteDbl = true;
			}
			else if(execMeArg.result_get()->getType() == InternalType::RealBool)
			{
				Bool *pB			= execMeArg.result_get()->getAsBool();
				int *piB			= pB->bool_get();

				//find true item count
				int iItemCount = 0;
				for(int j = 0 ; j < pB->size_get() ; j++)
				{
					if(piB[j])
					{
						iItemCount++;
					}
				}

				//allow new Double variable
				pDbl					= new Double(iItemCount, 1);
				double* pdbl	= pDbl->real_get();

				int j = 0;
				for(int l = 0 ; l < pB->size_get() ; l++)
				{
					if(piB[l])
					{
						pdbl[j++] = l + 1;
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
					pDbl	= pIn->getAsDouble();//
				}
				else
				{//Heu ... ?
				}
			}

			double *pData = pDbl->real_get();

			piTabsize[k] = pDbl->size_get();
			piIndexList[k] = new int[piTabsize[k]];

			(*_piMaxDim)[k] = static_cast<int>(pData[0] + 0.5);
			int iSize = pDbl->size_get();
			if(_iDimSize != NULL)
			{
				_iDimSize[k] = iSize;
			}

			for(int j = 0 ; j < iSize ; j++)
			{
				piIndexList[k][j] = static_cast<int>(pData[j] + 0.5);
				if(piIndexList[k][j] > (*_piMaxDim)[k])
				{
					(*_piMaxDim)[k] = piIndexList[k][j];
				}
			}
			iTotalCombi *= iSize;

			if(bDeleteDbl == true)
			{
				delete pDbl;
			}
		}

		int iTabsize	= iTotalCombi * iProductElem;
		*_piIndexSeq	= new int[iTabsize];

		if(iTabsize > 1)
		{
			ExpandList(piIndexList, piTabsize, iProductElem, *_piIndexSeq);
		}
		else
		{
			_piIndexSeq[0][0] = piIndexList[0][0];
		}

		delete [] piTabsize;

		for(int l = 0 ; l < iProductElem ; l++)
		{
			delete[] piIndexList[l];
		}
		delete[] piIndexList;
		return iTotalCombi;
	}
}
