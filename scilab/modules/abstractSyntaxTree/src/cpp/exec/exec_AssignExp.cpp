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
using ast::Exp;
using ast::ExecVisitor;

//std::vector<std::vector<int>> ExpandList(std::vector<std::vector<int>> *_List);

namespace ast
{
	void ExecVisitor::visit (const AssignExp  &e)
	{
		symbol::Context *pcontext = symbol::Context::getInstance();
		/*Create local exec visitor*/
		ExecVisitor* execMeR = new ExecVisitor();
		try
		{
			/*getting what to assign*/
			e.right_exp_get().accept(*execMeR);

			/*get symbol*/
			const CallExp *CallVar		= dynamic_cast<const CallExp*>(&e.left_exp_get());
			InternalType *dataOld, *dataNew	= NULL;
			bool bNew			= false;
			const SimpleVar *Var, *propName;
			ObjectMatrix *object;
			{
				// To sum up, 4 possibilities for the left expression:
				//  (1) The modified object is a variable or (2) an object/structure property
				//  (3) The modified object is fully modified or (3) only a subscript of the matrix is modified
				//  (1) => Var != NULL, object = NULL, propName = NULL; (2) => the opposite
				//  (3) => CallVar = NULL; (4) => CallVar != NULL
				const Exp *ref;
				if(CallVar != NULL)
				{ // e = foo(...) => CallVar = e, ref = foo
					ref = &CallVar->name_get();
				}
				else
				{ // e = var => CallVar = NULL, ref = e
					ref = &e.left_exp_get();
				}
				
				Var = dynamic_cast<const SimpleVar*>(ref);
				if(Var != NULL)
				{ // ref = foo => Var = Foo, object & propName = NULL
					object = NULL;
					propName = NULL;
					dataOld = symbol::Context::getInstance()->get(Var->name_get());
				}
				else
				{ // ref = foo.bar => Var = NULL, object = foo, propName = bar
					ExecVisitor execMeF;
					
					const FieldExp *field = dynamic_cast<const FieldExp*>(ref);
					field->head_get()->accept(execMeF);
					
					object = dynamic_cast<ObjectMatrix*>(execMeF.result_get());
					propName = dynamic_cast<const SimpleVar*>(field->tail_get());
					
					dataOld = object->get(propName->name_get().name_get());
				}
			}
			
			if(CallVar != NULL)
			{//dynamic_cast successed : left operand is not a SimpleVar ;)
				int iProductElem				= (int)CallVar->args_get().size();
				InternalType *pIT				= dataOld;
				bool bSeeAsVector				= iProductElem == 1;
				
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
				int iTotalCombi		= GetIndexList(CallVar->args_get(), &piIndexSeq, &piMaxDim, pIT, piDimSize);

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
									string szErr(os.str());
									throw szErr;
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

						dataNew = pOut;
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

				delete piMaxDim;
				delete[] piDimSize;
			}
			else
			{//CallVar != NULL
				InternalType *pVar	=	execMeR->result_get();
				if(pVar->isList())
				{
					double *pData = NULL;
					Double *pTemp = new Double(1, ((ImplicitList*)pVar)->size_get(), &pData);
					((ImplicitList*)pVar)->extract_matrix(pData);
					delete pVar;
					pVar = pTemp;
				}
				dataNew = pVar;
				bNew = true;
			}
			
			if(bNew)
			{
				if(Var != NULL)
				{
					symbol::Context::getInstance()->put(Var->name_get(), *((GenericType*)dataNew));
				}
				else
				{
					object->set(propName->name_get().name_get(), dataNew);
				}
			}

			if(e.is_verbose())
			{
				std::ostringstream ostr;
				if(Var != NULL)
				{
					ostr << Var->name_get() << " = " << std::endl;
				}
				else
				{
					ostr << object->toString(10,75) << "." << propName->name_get() << " = " << std::endl;
				}
				ostr << std::endl;
				ostr << dataNew->toString(10,75) << std::endl;
				YaspWrite((char *)ostr.str().c_str());
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
