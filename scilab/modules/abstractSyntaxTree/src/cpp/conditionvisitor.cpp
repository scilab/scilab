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

#include "conditionvisitor.hxx"
#include "originalvisitor.hxx"
#include "timer.hxx"
#include "localization.h"
#include "context.hxx"

#include "yaspio.hxx"

using std::string;

namespace ast
{
	void ConditionVisitor::visit(const OpExp &e)
	{
		bool bResult					= false;

		OriginalVisitor execMeL;
		OriginalVisitor execMeR;

		/*getting what to assign*/
		e.left_get().accept(execMeL);
		/*getting what to assign*/
		e.right_get().accept(execMeR);
		GenericType::RealType TypeL = execMeL.result_get()->getType();
		GenericType::RealType TypeR = execMeR.result_get()->getType();

		if(TypeL == GenericType::RealImplicitList)
		{
			ImplicitList* pIL = execMeL.result_get()->getAsImplicitList();
			if(pIL->computable())
			{
				InternalType *pIT = pIL->extract_matrix();
				execMeL.result_set(pIT);
				TypeL = pIT->getType();
			}
		}

		if(TypeR == GenericType::RealImplicitList)
		{
			ImplicitList* pIL = execMeR.result_get()->getAsImplicitList();
			if(pIL->computable())
			{
				InternalType* pIT = pIL->extract_matrix();
				execMeR.result_set(pIT);
				TypeR = pIT->getType();
			}
		}

		switch(e.oper_get())
		{
		case OpExp::eq :
			if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
			{
				Double *pL			= execMeL.result_get()->getAsDouble();
				Double *pR			= execMeR.result_get()->getAsDouble();

				if(pR->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pR->real_get(0,0);
					for(int i = 0 ; i < pL->rows_get() ; i++)
					{
						for(int j = 0 ; j < pL->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) == dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pL->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pL->real_get(0,0);
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pR->real_get(i, j) == dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
				{
					bResult = true;
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) == pR->real_get(i, j));
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else
				{
					bResult = false;
				}

				result_bool_set(bResult);
			}
			break;
		case OpExp::ne :
			if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
			{
				Double *pL			= execMeL.result_get()->getAsDouble();
				Double *pR			= execMeR.result_get()->getAsDouble();

				if(pR->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pR->real_get(0,0);
					for(int i = 0 ; i < pL->rows_get() ; i++)
					{
						for(int j = 0 ; j < pL->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) != dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pL->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pL->real_get(0,0);
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pR->real_get(i, j) != dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
				{
					bResult = true;
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) != pR->real_get(i, j));
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else
				{
					bResult = false;
				}

				result_bool_set(bResult);
			}
			break;
		case OpExp::gt :
			if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
			{
				Double *pL			= execMeL.result_get()->getAsDouble();
				Double *pR			= execMeR.result_get()->getAsDouble();

				if(pR->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pR->real_get(0,0);
					for(int i = 0 ; i < pL->rows_get() ; i++)
					{
						for(int j = 0 ; j < pL->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) > dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pL->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pL->real_get(0,0);
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pR->real_get(i, j) > dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
				{
					bResult = true;
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) > pR->real_get(i, j));
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else
				{
					bResult = false;
				}

				result_bool_set(bResult);
			}
			break;
		case OpExp::ge :
			if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
			{
				Double *pL			= execMeL.result_get()->getAsDouble();
				Double *pR			= execMeR.result_get()->getAsDouble();

				if(pR->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pR->real_get(0,0);
					for(int i = 0 ; i < pL->rows_get() ; i++)
					{
						for(int j = 0 ; j < pL->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) >= dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pL->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pL->real_get(0,0);
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pR->real_get(i, j) >= dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
				{
					bResult = true;
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) >= pR->real_get(i, j));
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else
				{
					bResult = false;
				}

				result_bool_set(bResult);
			}
			break;
		case OpExp::lt :
			if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
			{
				Double *pL			= execMeL.result_get()->getAsDouble();
				Double *pR			= execMeR.result_get()->getAsDouble();

				if(pR->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pR->real_get(0,0);
					for(int i = 0 ; i < pL->rows_get() ; i++)
					{
						for(int j = 0 ; j < pL->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) < dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pL->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pL->real_get(0,0);
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pR->real_get(i, j) < dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
				{
					bResult = true;
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) < pR->real_get(i, j));
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else
				{
					bResult = false;
				}

				result_bool_set(bResult);
			}
			break;
		case OpExp::le :
			if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
			{
				Double *pL			= execMeL.result_get()->getAsDouble();
				Double *pR			= execMeR.result_get()->getAsDouble();

				if(pR->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pR->real_get(0,0);
					for(int i = 0 ; i < pL->rows_get() ; i++)
					{
						for(int j = 0 ; j < pL->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) <= dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pL->size_get() == 1)
				{
					bResult 			= true;
					double dblRef	= pL->real_get(0,0);
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pR->real_get(i, j) <= dblRef);
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
				{
					bResult = true;
					for(int i = 0 ; i < pR->rows_get() ; i++)
					{
						for(int j = 0 ; j < pR->cols_get() ; j++)
						{
							bResult &= (pL->real_get(i, j) <= pR->real_get(i, j));
							if(bResult == false)
							{
								break;
							}
						}
					}
				}
				else
				{
					bResult = false;
				}

				result_bool_set(bResult);
			}
			break;
		default :
			OriginalVisitor exec;
			e.accept(exec);
			vector<types::InternalType*>* vect = exec.result_list_get();
			for(uint i = 0 ; i < vect->size() ; i++)
			{
				result_set(i, (*vect)[i]);
			}
			break;
		}
	}

	void ConditionVisitor::result_bool_set(bool _bResult)
	{
		m_bBooleanManagement = true;
		this->m_bResult = _bResult;
	}

}
