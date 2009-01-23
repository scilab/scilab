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


#include "timer.hxx"
#include "execvisitor.hxx"
#include "core_math.h"

#include "types_multiplication.hxx"
#include "types_addition.hxx"
#include "types_substraction.hxx"
#include "types_divide.hxx"

using std::string;

namespace ast
{
	void ExecVisitor::visit(const OpExp &e)
	{
		ExecVisitor execMeL = *new ast::ExecVisitor();
		ExecVisitor execMeR = *new ast::ExecVisitor();

		/*getting what to assign*/
		e.left_get().accept(execMeL);
		/*getting what to assign*/
		e.right_get().accept(execMeR);
		GenericType::RealType TypeL = execMeL.result_get()->getType();
		GenericType::RealType TypeR = execMeR.result_get()->getType();

		InternalType *pResult = NULL;
		switch(e.oper_get())
		{
		case OpExp::plus :
			{
				if(TypeR == GenericType::RealDouble && TypeL == GenericType::RealDouble)
				{
					Double *pL = execMeL.result_get()->getAsDouble();
					Double *pR = execMeR.result_get()->getAsDouble();

					int iResult = AddDoubleToDouble(pL, pR, (Double**)&pResult);
					if(iResult != 0)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
				{
					//nothing to do, all in macro : %b_+_b
				}
				else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
				{
					String *pL = execMeL.result_get()->getAsString();
					String *pR = execMeR.result_get()->getAsString();

					int iResult = AddStringToString(pL, pR, (String**)&pResult);

					if(iResult != 0)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealUInt && TypeR == GenericType::RealUInt)
				{
				}
				else if(TypeL == GenericType::RealInt && TypeR == GenericType::RealInt)
				{
				}
				else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
				{
					Double *pL				= execMeL.result_get()->getAsDouble();
					MatrixPoly *pR		= execMeR.result_get()->getAsPoly();

					int iResult = AddDoubleToPoly(pR, pL, (MatrixPoly**)&pResult);
					if(iResult != 0)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
				{
					Double *pR				= execMeR.result_get()->getAsDouble();
					MatrixPoly *pL		= execMeL.result_get()->getAsPoly();

					int iResult = AddDoubleToPoly(pL, pR, (MatrixPoly**)&pResult);
					if(iResult != 0)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
				{
					MatrixPoly *pL	= execMeL.result_get()->getAsPoly();
					MatrixPoly *pR	= execMeR.result_get()->getAsPoly();

					int iResult = AddPolyToPoly(pL, pR, (MatrixPoly**)&pResult);
					if(iResult != 0)
					{
						if(iResult == 1)
						{
							std::ostringstream os;
							os << "inconsistent row/column dimensions";
							os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
							string szErr(os.str());
							throw szErr;
						}
						else if(iResult == 2)
						{
							std::ostringstream os;
							os << "variables don't have the same formal variable";
							os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
							string szErr(os.str());
							throw szErr;
						}
					}
					result_set(pResult);
				}
				break;
			}
		case OpExp::minus :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL = execMeL.result_get()->getAsDouble();
					Double *pR = execMeR.result_get()->getAsDouble();

					int iResult = SubstractDoubleToDouble(pL, pR, (Double**)&pResult);
					if(iResult != 0)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealPoly)
				{
					Double *pL					= execMeL.result_get()->getAsDouble();
					MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

					int iResult = SubstractPolyToDouble(pL, pR, (MatrixPoly**)&pResult);
					if(iResult != 0)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
				{
					MatrixPoly *pL			= execMeL.result_get()->getAsPoly();
					Double *pR					= execMeR.result_get()->getAsDouble();

					int iResult = SubstractDoubleToPoly(pL, pR, (MatrixPoly**)&pResult);
					if(iResult != 0)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealPoly)
				{
					MatrixPoly *pL			= execMeL.result_get()->getAsPoly();
					MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

					int iResult = SubstractPolyToPoly(pL, pR, (MatrixPoly**)&pResult);
					if(iResult != 0)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealBool && TypeR == GenericType::RealBool)
				{
				}
				else if(TypeL == GenericType::RealString && TypeR == GenericType::RealString)
				{
				}
				else if(TypeL == GenericType::RealUInt && TypeR == GenericType::RealUInt)
				{
				}
				else if(TypeL == GenericType::RealInt && TypeR == GenericType::RealInt)
				{
				}

				break;
			}
		case OpExp::times:
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();

					int iRowResult 	= 0;
					int iColResult	= 0;

					if(pL->size_get() == 1)
					{
						iRowResult = pR->rows_get();
						iColResult = pR->cols_get();
					}
					else if (pR->size_get() == 1)
					{
						iRowResult = pL->rows_get();
						iColResult = pL->cols_get();
					}
					else if(pL->cols_get() == pR->rows_get())
					{
						iRowResult = pL->rows_get();
						iColResult = pR->cols_get();
					}

					pResult = new Double(iRowResult, iColResult, pL->isComplex() || pR->isComplex());

					int iResult = MultiplyDoubleByDouble(pL, pR, pResult->getAsDouble());
					if(iResult)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}

					result_set(pResult);
				}
				else if(TypeL == InternalType::RealDouble && TypeR == InternalType::RealPoly)
				{
					Double *pL			    = execMeL.result_get()->getAsDouble();
					MatrixPoly *pR	    = execMeR.result_get()->getAsPoly();

					int iRowResult 	= 0;
					int iColResult	= 0;
					int *piRank			= NULL;
					if(pL->size_get() == 1)
					{
						iRowResult = pR->rows_get();
						iColResult = pR->cols_get();

						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pR->poly_get(i)->rank_get();
						}
					}
					else if (pR->size_get() == 1)
					{
						iRowResult = pL->rows_get();
						iColResult = pL->cols_get();

						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pR->poly_get(0)->rank_get();
						}
					}
					else if(pL->cols_get() == pR->rows_get())
					{
						iRowResult = pL->rows_get();
						iColResult = pR->cols_get();
						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pR->rank_max_get();
						}
					}

					pResult = new MatrixPoly(pR->var_get(), iRowResult, iColResult, piRank);
					delete piRank;
					if(pL->isComplex() || pR->isComplex())
					{
						pResult->getAsPoly()->complex_set(true);
					}

					int iResult = MultiplyDoubleByPoly(pL, pR, pResult->getAsPoly());

					if(iResult)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}

					result_set(pResult);
				}
				else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealDouble)
				{
					MatrixPoly *pL	    = execMeL.result_get()->getAsPoly();
					Double *pR			    = execMeR.result_get()->getAsDouble();

					int iRowResult 	= 0;
					int iColResult	= 0;
					int *piRank			= NULL;
					if(pL->size_get() == 1)
					{
						iRowResult = pR->rows_get();
						iColResult = pR->cols_get();

						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pL->poly_get(0)->rank_get();
						}
					}
					else if (pR->size_get() == 1)
					{
						iRowResult = pL->rows_get();
						iColResult = pL->cols_get();

						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pL->poly_get(i)->rank_get();
						}
					}
					else if(pL->cols_get() == pR->rows_get())
					{
						iRowResult = pL->rows_get();
						iColResult = pR->cols_get();
						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pL->rank_max_get();
						}
					}

					pResult = new MatrixPoly(pL->var_get(), iRowResult, iColResult, piRank);
					delete piRank;
					if(pL->isComplex() || pR->isComplex())
					{
						pResult->getAsPoly()->complex_set(true);
					}

					int iResult = MultiplyPolyByDouble(pL, pR, pResult->getAsPoly());

					if(iResult)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}

					result_set(pResult);
				}
				else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealPoly)
				{
					timer timed;
					MatrixPoly *pL	    = execMeL.result_get()->getAsPoly();
					MatrixPoly *pR			= execMeR.result_get()->getAsPoly();

					int iRowResult 	= 0;
					int iColResult	= 0;
					int *piRank			= NULL;

					if(pL->size_get() == 1 && pR->size_get() == 1)
					{
						iRowResult = 1;
						iColResult = 1;

						piRank = new int[1];
						piRank[0] = pL->poly_get(0)->rank_get() + pR->poly_get(0)->rank_get() - 1;
					}
					else if(pL->size_get() == 1)
					{
						iRowResult = pR->rows_get();
						iColResult = pR->cols_get();

						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pL->poly_get(0)->rank_get() + pR->poly_get(i)->rank_get() - 1;
						}
					}
					else if (pR->size_get() == 1)
					{
						iRowResult = pL->rows_get();
						iColResult = pL->cols_get();

						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pR->poly_get(0)->rank_get() * pL->poly_get(i)->rank_get();
						}
					}
					else if(pL->cols_get() == pR->rows_get())
					{
						iRowResult = pL->rows_get();
						iColResult = pR->cols_get();
						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pL->rank_max_get() * pR->rank_max_get();
						}
					}

					pResult = new MatrixPoly(pL->var_get(), iRowResult, iColResult, piRank);

					delete[] piRank;

					if(pL->isComplex() || pR->isComplex())
					{
						pResult->getAsPoly()->complex_set(true);
					}

					int iResult = MultiplyPolyByPoly(pL, pR, pResult->getAsPoly());

					if(iResult)
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}

					result_set(pResult);
				}
				break;
			}
		case OpExp::divide:
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();

					int iResult = DivideDoubleByDouble(pL, pR, (Double**)&pResult);
					if(iResult)
					{//manage errors
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeL == GenericType::RealPoly && TypeR == GenericType::RealDouble)
				{
					MatrixPoly *pL	= execMeL.result_get()->getAsPoly();
					Double *pR			= execMeR.result_get()->getAsDouble();
					int iRowResult 	= 0;
					int iColResult	= 0;
					int *piRank			= NULL;

					if(pL->size_get() == 1)
					{
						iRowResult = pR->rows_get();
						iColResult = pR->cols_get();

						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pL->poly_get(0)->rank_get();
						}
					}
					else if(pR->size_get() == 1)
					{
						iRowResult = pL->rows_get();
						iColResult = pL->cols_get();

						piRank = new int[iRowResult * iColResult];
						for(int i = 0 ; i < iRowResult * iColResult ; i++)
						{
							piRank[i] = pL->poly_get(i)->rank_get();
						}
					}
					else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
					{//Je ne sais pas encore comment ca marche ce machin la !!!
						iRowResult = pR->rows_get();
						iColResult = pR->cols_get();
					}

					pResult = new MatrixPoly(pL->var_get(), iRowResult, iColResult, piRank);

					DividePolyByDouble(pL, pR, pResult->getAsPoly());
				}
				break;
			}
		case OpExp::eq :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) == dblRef);
							}
						}
					}
					else if(pL->size_get() == 1)
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						double dblRef	= pL->real_get(0,0);
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, dblRef == pR->real_get(i, j));
							}
						}
					}
					else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) == pR->real_get(i, j));
							}
						}
					}
					else
					{
						pResult = new Bool(false);
					}

					result_set(pResult);
				}
				break;
			}
		case OpExp::ne :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) != dblRef);
							}
						}
					}
					else if(pL->size_get() == 1)
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						double dblRef	= pL->real_get(0,0);
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, dblRef != pR->real_get(i, j));
							}
						}
					}
					else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) != pR->real_get(i, j));
							}
						}
					}
					else
					{
						pResult = new Bool(false);
					}

					result_set(pResult);
				}
				break;
			}
		case OpExp::lt :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) < dblRef);
							}
						}
					}
					else if(pL->size_get() == 1)
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						double dblRef	= pL->real_get(0,0);
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, dblRef < pR->real_get(i, j));
							}
						}
					}
					else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) < pR->real_get(i, j));
							}
						}
					}
					else
					{
						pResult = new Bool(false);
					}

					result_set(pResult);
				}
				break;
			}
		case OpExp::le :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) <= dblRef);
							}
						}
					}
					else if(pL->size_get() == 1)
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						double dblRef	= pL->real_get(0,0);
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, dblRef <= pR->real_get(i, j));
							}
						}
					}
					else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) <= pR->real_get(i, j));
							}
						}
					}
					else
					{
						pResult = new Bool(false);
					}

					result_set(pResult);
				}
				break;
			}
		case OpExp::gt :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) > dblRef);
							}
						}
					}
					else if(pL->size_get() == 1)
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						double dblRef	= pL->real_get(0,0);
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, dblRef > pR->real_get(i, j));
							}
						}
					}
					else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());

						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) > pR->real_get(i, j));
							}
						}
					}
					else
					{
						pResult = new Bool(false);
					}

					result_set(pResult);
				}
				break;
			}
		case OpExp::ge :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) >= dblRef);
							}
						}
					}
					else if(pL->size_get() == 1)
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						double dblRef	= pL->real_get(0,0);
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, dblRef >= pR->real_get(i, j));
							}
						}
					}
					else if(pR->rows_get() == pL->rows_get() && pR->cols_get() == pL->cols_get())
					{
						pResult				= new Bool(pR->rows_get(), pR->cols_get());
						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								pResult->getAsBool()->bool_set(i, j, pL->real_get(i, j) >= pR->real_get(i, j));
							}
						}
					}
					else
					{
						pResult = new Bool(false);
					}

					result_set(pResult);
				}
				break;
			}
		case OpExp::power :
			break;
		default :
			break;
		}

		if(e.is_verbose())
		{
			std::cout << pResult->toString(10,75) << std::endl;
		}
	}
}

