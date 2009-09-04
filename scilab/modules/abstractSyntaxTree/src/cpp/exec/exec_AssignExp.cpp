/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "execvisitor.hxx"
#include "context.hxx"
#include <string>

#include "yaspio.hxx"

using std::string;
using ast::Exp;
using ast::ExecVisitor;

namespace ast
{
	void ExecVisitor::visit (const AssignExp  &e)
	{
		symbol::Context *pcontext = symbol::Context::getInstance();
		/*Create local exec visitor*/
		ExecVisitor* execMeR = new ExecVisitor();
		try
		{
			/*get king of left hand*/
			const SimpleVar *pVar				= dynamic_cast<const SimpleVar*>(&e.left_exp_get());
			const AssignListExp *pList	= dynamic_cast<const AssignListExp*>(&e.left_exp_get());
			const CallExp *pCall				= dynamic_cast<const CallExp*>(&e.left_exp_get());

			if(pCall)
			{//x(?) = ?
				bool bNew								= false;
				int iProductElem				= (int)pCall->args_get().size();
				pVar										= (SimpleVar*)&pCall->name_get();
				InternalType *pIT				= symbol::Context::getInstance()->get(pVar->name_get());
				bool bSeeAsVector				= iProductElem == 1;

				/*getting what to assign*/
				e.right_exp_get().accept(*execMeR);
				if(pIT == NULL)
				{//Var doesn't exist, create it with good dimensions
					bNew = true;
				}
				else
				{
					if(pIT->isRef(1) == true)
					{
						pIT = pIT->clone();
					}
				}

				int *piIndexSeq		= NULL;
				int *piMaxDim			= NULL;
				int *piDimSize			= new int[iProductElem];
				int iTotalCombi		= GetIndexList(pCall->args_get(), &piIndexSeq, &piMaxDim, pIT, piDimSize);

				/*I have  the indexlist expanded and the max index*/

				if(execMeR->result_get()->getType() == InternalType::RealDouble)
				{
					Double *pOut	= NULL;
					Double *pIn		= execMeR->result_get()->getAsDouble();

					//tips : if pIT == NULL then pOld = NULL
					Double *pOld	= pIT != NULL ? pIT->getAsDouble() : NULL;

					if(bSeeAsVector == false)
					{
						//pOld can be null but only if bNew == true, in this case pOld->xxx was never evaluate
						//if Index is a vector, we can enalge Out only if it's a vector
						if(bNew || pOld->rows_get() < piMaxDim[0] || pOld->cols_get() < piMaxDim[1])
						{//new or smaller
							if(bNew == false)
							{
								pOut = new Double(Max(piMaxDim[0], pOld->rows_get()), Max(piMaxDim[1], pOld->cols_get()), pIn->isComplex());
							}
							else
							{
								pOut = new Double(piMaxDim[0], piMaxDim[1], pIn->isComplex());
							}
								pOut->zero_set();

							if(bNew == false)
							{//copy old values
								//toggle this flag to add var is the scope
								bNew = true;
								if(pOld->isComplex())
								{//if pOut is already complex, complex_set function exits without change anything
									pOut->complex_set(true);
								}

								double *pOldR = pOld->real_get();
								double *pOutR	= pOut->real_get();
								int iRowOut		= pOut->rows_get();
								int iRowOld		= pOld->rows_get();
								int iColOld		= pOld->cols_get();

								if(pOut->isComplex())
								{
									double *pOldI = pOld->img_get();
									double *pOutI	= pOut->img_get();

									for(int i = 0 ; i < iRowOld ; i++)
									{
										for(int j = 0 ; j < iColOld ; j++)
										{
											pOutR[j * iRowOut + i] = pOldR[j * iRowOld + i];
											pOutI[j * iRowOut + i] = pOldI[j * iRowOld + i];
										}
									}
								}
								else
								{
									for(int i = 0 ; i < iRowOld ; i++)
									{
										for(int j = 0 ; j < iColOld ; j++)
										{
											pOutR[j * iRowOut + i] = pOldR[j * iRowOld + i];
										}
									}
								}
							}
						}
						else
						{
							pOut = pIT->getAsDouble();
						}
					}
					else
					{//bSeeAsVector
						if(bNew)
						{
							if(pIn->cols_get() == 1)
							{
								pOut = new Double(piMaxDim[0], 1, pIn->isComplex());
								pOut->zero_set();
							}
							else if(pIn->rows_get() == 1)
							{
								pOut = new Double(1, piMaxDim[0], pIn->isComplex());
								pOut->zero_set();
							}
							else
							{
								std::ostringstream os;
								os << "inconsistent row/column dimensions";
								os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
								throw os.str();
							}
						}
						else
						{//Out already exist
							if(pIn->size_get() != 1 && pIn->cols_get() != 1 && pIn->rows_get() != 1 && pIn->size_get() == iTotalCombi)
							{//bad size
								std::ostringstream os;
								os << "inconsistent row/column dimensions";
								os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
								throw os.str();
							}

							if(pOld->size_get() >= piMaxDim[0])
							{//pOut bigger than pIn
								pOut = pIT->getAsDouble();
							}
							else
							{//pOut smaller than pIn, enlarge pOut
								bNew = true;
								if(pOld->cols_get() == 1 || pOld->size_get() == 0)
								{
									pOut = new Double(piMaxDim[0], 1, pIn->isComplex());
								}
								else if(pOld->rows_get() == 1)
								{
									pOut = new Double(1, piMaxDim[0], pIn->isComplex());
								}
								else
								{
									std::ostringstream os;
									os << "inconsistent row/column dimensions";
									os << " (" << e.left_exp_get().location_get().first_line << "," << e.left_exp_get().location_get().first_column << ")" << std::endl;
									throw os.str();
								}

								double *pOldR = pOld->real_get();
								double *pOutR = pOut->real_get();

								if(pOld->isComplex())
								{
									double *pOldI = pOld->img_get();
									double *pOutI = pOut->img_get();

									pOut->complex_set(true);
									pOut->zero_set();

									for(int i = 0 ; i < pOld->size_get() ; i++)
									{
										pOutR[i] = pOldR[i];
										pOutI[i] = pOldI[i];
									}

								}
								else
								{
									pOut->zero_set();
									for(int i = 0 ; i < pOld->size_get() ; i++)
									{
										pOutR[i] = pOldR[i];
									}
								}
							}
						}
					}

					//check if the size of In is compatible with the size of Out
					if(pIn->rows_get() <= pOut->rows_get() && pIn->cols_get() <= pOut->cols_get() &&
						(pIn->size_get() == 1 || pIn->size_get() == iTotalCombi))
					{
						//Copy values in pOut at indexed
						double *pInR	= pIn->real_get();
						double *pOutR = pOut->real_get();

						if(pIn->size_get() == 1)
						{//a(?) = C
							if(pIn->isComplex())
							{//a([]) = C
								pOut->complex_set(true);
								double *pInI	= pIn->img_get();
								for(int i = 0 ; i < iTotalCombi ; i ++)
								{
								}
							}
							else
							{
								if(bSeeAsVector == true)
								{//a([]) = R
									for(int j = 0 ; j < iTotalCombi ; j++)
									{
										int iPos = piIndexSeq[j] - 1;
										pOutR[iPos] = pInR[0];
									}
								}
								else
								{//a([],[]) = R
									int iRowOut		= pOut->rows_get();
									for(int i = 0 ; i < iTotalCombi ; i ++)
									{
										int iRow		= piIndexSeq[i * iProductElem] - 1;
										int iCol		= piIndexSeq[i * iProductElem + 1] - 1;
										int iPos		= iRow + iCol * iRowOut;
										pOutR[iPos] = pInR[0];
									}
								}
							}
						}
						else
						{//a(?) = [C]
							if(pIn->isComplex())
							{//a(?) = [C]
								pOut->complex_set(true);
								double *pInI	= pIn->img_get();
								for(int i = 0 ; i < iTotalCombi ; i ++)
								{
								}
							}
							else
							{
								if(bSeeAsVector)
								{//a([]) = [R]
									for(int i = 0 ; i < iTotalCombi ; i ++)
									{
										int iPos = piIndexSeq[i * iProductElem] - 1;
										pOutR[iPos] = pInR[i];
									}
								}
								else
								{//a([][]) = [R]
									int iRowOut		= pOut->rows_get();
									int iRowIn		= pIn->rows_get();
									int iColIn		= pIn->cols_get();

									for(int i = 0 ; i < iTotalCombi ; i ++)
									{
										int iRow		= piIndexSeq[i * iProductElem] - 1;
										int iCol		= piIndexSeq[i * iProductElem + 1] - 1;
										int iPos		= iRow + iCol * iRowOut;

										//try to "linearise pInR Oo
										int iTempR = i / iColIn;
										int iTempC = i % iColIn;
										int iNew_i = iTempR + iTempC * iRowIn;

										pOutR[iPos] = pInR[iNew_i];
									}
								}
							}
						}

						if(bNew)
						{
							symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pOut));
						}

						if(e.is_verbose())
						{
							std::ostringstream ostr;
							ostr << pVar->name_get() << " = " << std::endl;
							ostr << std::endl;
							ostr << pOut->toString(10,75) << std::endl;
							YaspWrite((char *)ostr.str().c_str());
						}
					}
					else
					{//manage error
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
						throw os.str();
					}
				}
				else if(execMeR->result_get()->getType() == InternalType::RealPoly)
				{//other tpyes
					getchar();
				}

				delete piMaxDim;
				delete[] piDimSize;
			}
			else if(pVar)
			{// x = ?
				/*getting what to assign*/
				e.right_exp_get().accept(*execMeR);

				if(execMeR->result_size_get() != 1)
				{
					std::ostringstream os;
					os << "Lhs != Rhs";
					os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
					throw os.str();
				}
				
				InternalType *pIT	=	execMeR->result_get();
				if(pIT->isImplicitList())
				{
					double *pData = NULL;
					Double *pTemp = new Double(1, ((ImplicitList*)pIT)->size_get(), &pData);
					((ImplicitList*)pIT)->extract_matrix(pData);
					delete pIT;
					pIT = pTemp;
				}

				symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pIT));

				if(e.is_verbose())
				{
					std::ostringstream ostr;
					ostr << pVar->name_get() << " = " << std::endl;
					ostr << std::endl;
					ostr << pIT->toString(10,75) << std::endl;
					YaspWrite((char *)ostr.str().c_str());
				}
			}
			else if(pList)
			{//[x,y] = ?

				size_t iLhsCount = pList->exps_get().size();

				/*getting what to assign*/
				execMeR->expected_size_set(iLhsCount);
				e.right_exp_get().accept(*execMeR);

				if(execMeR->result_size_get() != execMeR->expected_size_get())
				{
					std::ostringstream os;
					os << "Incorrect assignment" << std::endl;
					throw os.str();
				}


				std::list<Exp *>::const_reverse_iterator it;
				int i = (int)iLhsCount - 1;
				for(it = pList->exps_get().rbegin() ; it != pList->exps_get().rend() ; it++)
				{
					const SimpleVar *pListVar	= dynamic_cast<const SimpleVar*>((*it));
					symbol::Context::getInstance()->put(pListVar->name_get(), *((GenericType*)execMeR->result_get(i)));
					if(e.is_verbose())
					{
						std::ostringstream ostr;
						ostr << pListVar->name_get() << " = " << std::endl;
						ostr << std::endl;
						ostr << execMeR->result_get(i)->toString(10,75) << std::endl;
						YaspWrite((char *)ostr.str().c_str());
					}
					i--;
				}

			}
			else
			{//Houston ...
				std::ostringstream os;
				os << "unknow script form";
				os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
				throw os.str();
			}
		}
		catch(string sz)
		{
			throw sz;
		}
		delete execMeR;
	}
}

/*
std::vector<std::vector<int>> ExpandList(std::vector<std::vector<int>> *_List)
{
	std::vector<std::vector<int> > ReturnList;
	if(_List->size() == 1)
	{
		for(unsigned int i = 0 ; i< (*_List)[0].size() ; i++)
		{
			std::vector<int> SubList;
			SubList.push_back((*_List)[0][i]);
			ReturnList.push_back(SubList);
		}
	}
	else
	{
		std::vector<int> SubList;
		std::vector<std::vector<int> > TempList;
		SubList = *(_List->begin());
		_List->erase(_List->begin());
		TempList = ExpandList(_List);
		for(unsigned int k = 0 ; k < SubList.size() ; k++)
		{
			for(unsigned int j = 0 ; j < TempList.size() ; j++)
			{
				std::vector<int> SubList2;
				SubList2.push_back(SubList[k]);
				for(unsigned int i = 0 ; i < TempList[j].size() ; i++)
				{
					SubList2.push_back(TempList[j][i]);
				}
				ReturnList.push_back(SubList2);
			}
		}
	}
	return ReturnList;
}
*/
