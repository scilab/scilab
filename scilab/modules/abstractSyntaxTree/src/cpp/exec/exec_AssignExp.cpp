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

using std::string;

//std::vector<std::vector<int>> ExpandList(std::vector<std::vector<int>> *_List);
void ExpandList(int ** _piList, int *_piListSize, int _iListSizeSize, int *_piResultList);
int GetVarMaxDim(InternalType *_pIT, int _iCurrentDim, int _iMaxDim);

namespace ast
{
	void ExecVisitor::visit (const AssignExp  &e)
	{
		/*Create local exec visitor*/
		ExecVisitor* execMeR = new ast::ExecVisitor();
		try
		{
			/*getting what to assign*/
			e.right_exp_get().accept(*execMeR);

			/*get symbol*/
			const SimpleVar *Var = dynamic_cast<const SimpleVar*>(&e.left_exp_get());
			if(Var == NULL)
			{//dynamic_cast failed : left operand is not a SimpleVar ;)
				bool bNew = false;
				ExecVisitor* execMeArg = new ast::ExecVisitor();
				CallExp *CallVar = (CallExp *)(&e.left_exp_get());
				Var = (SimpleVar*)&CallVar->name_get();
				//Var = dynamic_cast<const SimpleVar*>(&CallVar->name_get());
				InternalType *pIT = symbol::Context::getInstance()->get(Var->name_get());
				bool bSeeAsVector	= CallVar->args_get().size() == 1;

				if(pIT == NULL)
				{//Var doesn't exist, create it with good dimensions
					bNew = true;
				}

				//Create list of indexes
				//std::vector<std::vector<int>> IndexList;
				int *piTabSize = new int[CallVar->args_get().size()];
				int *piMaxDim = new int[CallVar->args_get().size()];
				int **piIndexList = NULL;
				int iTotalCombi		= 1;

				piIndexList = new int*[CallVar->args_get().size()];


				int k = 0;
				std::list<Exp *>::const_iterator	i;
				for(i = CallVar->args_get().begin() ; i != CallVar->args_get().end() ; i++,k++)
				{
					(*i)->accept(*execMeArg);
					InternalType *pIn = NULL;
					Double *pDbl = NULL;
					bool bDeleteDbl = false;

					if(execMeArg->result_get()->getType() == InternalType::RealImplicitList)
					{//a:b:c
						int iMaxDim = 0;
						if(pIT != NULL)
						{
							iMaxDim = GetVarMaxDim(pIT, k, CallVar->args_get().size());
						}

						Double dbl(iMaxDim); // $
						ImplicitList *pIL = execMeArg->result_get()->getAsList();
						if(pIL->computable() == false)
						{
							if(pIL->start_type_get() == InternalType::RealPoly)
							{
								MatrixPoly *poPoly	= pIL->start_poly_get();
								pIL->start_set(poPoly->evaluate(&dbl)->real_get(0,0));
							}
							if(pIL->step_type_get() == InternalType::RealPoly)
							{
								MatrixPoly *poPoly	= pIL->step_poly_get();
								pIL->step_set(poPoly->evaluate(&dbl)->real_get(0,0));
							}
							if(pIL->end_type_get() == InternalType::RealPoly)
							{
								MatrixPoly *poPoly	= pIL->end_poly_get();
								pIL->end_set(poPoly->evaluate(&dbl)->real_get(0,0));
							}
						}

						double *pDblData = NULL;
						pDbl = new Double(1, pIL->size_get(), &pDblData);
						pIL->extract_matrix(pDblData);
						bDeleteDbl = true;
					}
					else
					{
						pIn = execMeArg->result_get();

						if(pIn->getType() == InternalType::RealPoly)
						{//manage $
							MatrixPoly *pPoly = pIn->getAsPoly();

							if(pIT != NULL)
							{
								int iMaxDim = GetVarMaxDim(pIT, k, CallVar->args_get().size());
								Double dbl(iMaxDim); // $
								pDbl = pPoly->evaluate(&dbl);
							}
							else
							{//houston we have a problem ...
								//Allow empty matrix
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

//					std::vector<int> SubList;
					piTabSize[k] = pDbl->size_get();
					piIndexList[k] = new int[piTabSize[k]];

					piMaxDim[k] = (int)(pData[0] + 0.5);
					int iSize = pDbl->size_get();
					for(int j = 0 ; j < iSize ; j++)
					{
						if((int)(pData[j] + 0.5) > piMaxDim[k])
						{
							piMaxDim[k] = (int)(pData[j] + 0.5);
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

				int iTabSize = iTotalCombi * CallVar->args_get().size();
				//std::vector<std::vector<int>> IndexSeq;
//				IndexSeq = ExpandList(&IndexList);
				int *piIndexSeq		= new int[iTabSize];
				int iProductElem	= CallVar->args_get().size();
				ExpandList(piIndexList, piTabSize, CallVar->args_get().size(), piIndexSeq);

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
							pOut = new Double(piMaxDim[0], piMaxDim[1], pIn->isComplex());

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
							}
							else if(pIn->rows_get() == 1)
							{
								pOut = new Double(1, piMaxDim[0], pIn->isComplex());
							}
							else
							{
								std::ostringstream os;
								os << "inconsistent row/column dimensions";
								os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
								string szErr(os.str());
								throw szErr;
							}
						}
						else
						{//Out already exist
							if(pIn->size_get() != 1 && pIn->cols_get() != 1 && pIn->rows_get() != 1 && pIn->size_get() == iTotalCombi)
							{//bad size
								std::ostringstream os;
								os << "inconsistent row/column dimensions";
								os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
								string szErr(os.str());
								throw szErr;
							}

							if(pOld->size_get() >= piMaxDim[0])
							{//pOut bigger than pIn
								pOut = pIT->getAsDouble();
							}
							else
							{//pOut smaller than pIn, enlarge pOut
								bNew = true;
								if(pOld->cols_get() == 1)
								{
									pOut = new Double(piMaxDim[0], 1, pIn->isComplex());
								}
								else if(pOld->rows_get() == 1)
								{
									pOut = new Double(1, piMaxDim[0], pIn->isComplex());
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
							symbol::Context::getInstance()->put(Var->name_get(), *((GenericType*)pOut));
						}
					}
					else
					{//manage error
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
				}
				else if(execMeR->result_get()->getType() == InternalType::RealPoly)
				{//other tpyes
					getchar();
				}

				delete piTabSize;
				delete piMaxDim;
				for(int i = 0 ; i < (int)CallVar->args_get().size() ; i++)
				{
					delete piIndexList[i];
				}
				delete piIndexList;
				delete execMeArg;
			}
			else
			{//Var == NULL
				InternalType *pVar	=	execMeR->result_get();
				if(pVar->isList())
				{
					double *pData = NULL;
					Double *pTemp = new Double(((ImplicitList*)pVar)->size_get(),1, &pData);
					((ImplicitList*)pVar)->extract_matrix(pData);
					delete pVar;
					pVar = pTemp;
				}

				symbol::Context::getInstance()->put(Var->name_get(), *((GenericType*)pVar));
				if(e.is_verbose())
				{
					std::cout << Var->name_get() << " = " << std::endl;
					std::cout << std::endl;
					std::cout << pVar->toString(10,75) << std::endl;
				}
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
