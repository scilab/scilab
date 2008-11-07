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

double GetMaxValue(double *_pdblData, int _iSize);
std::vector<std::vector<int>> ExpandList(std::vector<std::vector<int>> *_List);
int GetMaxDim(std::vector<std::vector<int>> *_List, unsigned int iOrder);

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
				Var = dynamic_cast<const SimpleVar*>(&CallVar->name_get());
				InternalType *pIT = symbol::Context::getInstance()->get(Var->name_get());
				bool bSeeAsVector	= CallVar->args_get().size() == 1;

				if(pIT == NULL)
				{//Var doesn't exist, create it with good dimensions
					bNew = true;
				}

				//Create list of indexes
				std::vector<std::vector<int>> IndexList;

				std::list<Exp *>::const_iterator	i;
				std::vector<int> MaxDim;
				for(i = CallVar->args_get().begin() ; i != CallVar->args_get().end() ; i++)
				{
					(*i)->accept(*execMeArg);
					Double *pDbl	= execMeArg->result_get()->getAsDouble();
					//indexed are matrix ou vector ?
					double *pData = pDbl->real_get();
					std::vector<int> SubList;
					MaxDim.push_back((int)pData[0]);
					for(int j = 0 ; j < pDbl->size_get() ; j++)
					{
						if((int)pData[j] > MaxDim[MaxDim.size()-1])
						{
							MaxDim[MaxDim.size()-1] = (int)pData[j];
						}
						SubList.push_back((int)pData[j]);
					}

					IndexList.push_back(SubList);
				}

				std::vector<std::vector<int>> IndexSeq;
				IndexSeq = ExpandList(&IndexList);

				/*I have, the indexlist expanded and the max index*/

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
						if(bNew || pOld->rows_get() < MaxDim[0] || pOld->cols_get() < MaxDim[1])
						{//new or smaller
							pOut = new Double(MaxDim[0], MaxDim[1], pIn->isComplex());

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

								if(pOut->isComplex())
								{
									double *pOldI = pOld->img_get();
									double *pOutI	= pOut->img_get();

									for(int i = 0 ; i < pOld->rows_get() ; i++)
									{
										for(int j = 0 ; j < pOld->cols_get() ; j++)
										{
											pOutR[j * pOut->rows_get() + i] = pOldR[j * pOld->rows_get() + i];
											pOutI[j * pOut->rows_get() + i] = pOldI[j * pOld->rows_get() + i];
										}
									}
								}
								else
								{
									for(int i = 0 ; i < pOld->rows_get() ; i++)
									{
										for(int j = 0 ; j < pOld->cols_get() ; j++)
										{
											pOutR[j * pOut->rows_get() + i] = pOldR[j * pOld->rows_get() + i];
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
								pOut = new Double(MaxDim[0], 1, pIn->isComplex());
							}
							else if(pIn->rows_get() == 1)
							{
								pOut = new Double(1, MaxDim[0], pIn->isComplex());
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
							if(pIn->size_get() != 1 && pIn->cols_get() != 1 && pIn->rows_get() != 1 && pIn->size_get() == IndexSeq.size())
							{//bad size
								std::ostringstream os;
								os << "inconsistent row/column dimensions";
								os << " (" << e.right_exp_get().location_get().first_line << "," << e.right_exp_get().location_get().first_column << ")" << std::endl;
								string szErr(os.str());
								throw szErr;
							}

							if(pOld->size_get() >= MaxDim[0])
							{//pOut bigger than pIn
								pOut = pIT->getAsDouble();
							}
							else
							{//pOut smaller than pIn, enlarge pOut
								bNew = true;
								if(pOld->cols_get() == 1)
								{
									pOut = new Double(MaxDim[0], 1, pIn->isComplex());
								}
								else if(pOld->rows_get() == 1)
								{
									pOut = new Double(1, MaxDim[0], pIn->isComplex());
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
						(pIn->size_get() == 1 || pIn->size_get() == IndexSeq.size()))
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
								for(unsigned int i = 0 ; i < IndexSeq.size() ; i ++)
								{
									std::vector<int> SubList = IndexList[i];

								}
							}
							else
							{
								if(bSeeAsVector == true)
								{//a([]) = R
									std::vector<int> SubList = IndexSeq[0];
									for(unsigned int j = 0 ; j < SubList.size() ; j++)
									{
										int iPos = SubList[j] - 1;
										pOutR[iPos] = pInR[0];
									}
								}
								else
								{//a([],[]) = R
									for(unsigned int i = 0 ; i < IndexSeq.size() ; i ++)
									{
										std::vector<int> SubList = IndexSeq[i];
										int iPos = (SubList[0] - 1) + (SubList[1] - 1) * pOut->rows_get();
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
								for(unsigned int i = 0 ; i < IndexSeq.size() ; i ++)
								{
									std::vector<int> SubList = IndexSeq[i];
								}
							}
							else
							{
								if(bSeeAsVector)
								{//a([]) = [R]
									for(unsigned int i = 0 ; i < IndexSeq.size() ; i ++)
									{
										std::vector<int> SubList = IndexSeq[i];
										int iPos = SubList[0] - 1;
										pOutR[iPos] = pInR[i];
									}
								}
								else
								{//a([][]) = [R]
									for(unsigned int i = 0 ; i < IndexSeq.size() ; i ++)
									{
										std::vector<int> SubList = IndexSeq[i];
										int iPos = (SubList[0] - 1) + (SubList[1] - 1) * pOut->rows_get();
										pOutR[iPos] = pInR[i];
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
				else
				{//other tpyes
				}
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
			}
		}
		catch(string sz)
		{
			throw sz;
		}
		delete execMeR;
	}
}


double GetMaxValue(double *_pdblData, int _iSize)
{
	if(_pdblData == NULL || _iSize <= 0)
	{
		return 0;
	}

	double dblMax = _pdblData[0];
	for(int i = 1 ; i < _iSize ; i++)
	{
		if(_pdblData[i] > dblMax)
			dblMax = _pdblData[i];
	}
	return dblMax;
}

int GetMaxDim(std::vector<std::vector<int>> *_List, unsigned int iOrder)
{
	int iRef = 0;
	for(unsigned int i = 0 ; i < _List->size() ; i++)
	{
		std::vector<int> SubList = (*_List)[i];
		if(SubList.size() < iOrder)
			return 0;

		int iVal = (*_List)[i][iOrder];
		if(iVal > iRef)
		{
			iRef = iVal;
		}
	}
	return iRef;
}

std::vector<std::vector<int>> ExpandList(std::vector<std::vector<int>> *_List)
{
	std::vector<std::vector<int>> ReturnList;
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
		std::vector<std::vector<int>> TempList;
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
