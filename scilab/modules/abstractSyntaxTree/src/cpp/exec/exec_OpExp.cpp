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

					pResult = AddDoubleToDouble(pL, pR);
					if(pResult == NULL)
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

					pResult = AddStringToString(pL, pR);

					if(pResult == NULL)
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

					pResult = AddDoubleToPoly(pR, pL);
					if(pResult == NULL)
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

					pResult = AddDoubleToPoly(pL, pR);
					if(pResult == NULL)
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

					if(pL->var_get() == pR->var_get())
					{
						pResult = AddPolyToPoly(pL, pR);
						if(pResult == NULL)
						{
							std::ostringstream os;
							os << "inconsistent row/column dimensions";
							os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
							string szErr(os.str());
							throw szErr;
						}
						result_set(pResult);
					}
					else
					{
						std::ostringstream os;
						os << "variables don't have the same formal variable";
						os << " (" << e.right_get().location_get().first_line << "," << e.right_get().location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
				}
				break;
			}
		case OpExp::minus :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL = execMeL.result_get()->getAsDouble();
					Double *pR = execMeR.result_get()->getAsDouble();

					pResult = SubstractDoubleToDouble(pL, pR);
					if(pResult == NULL)
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

					pResult = SubstractPolyToDouble(pL, pR);
					if(pResult == NULL)
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

					pResult = SubstractDoubleToPoly(pL, pR);
					if(pResult == NULL)
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

					pResult = SubstractPolyToPoly(pL, pR);
					if(pResult == NULL)
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
					int iRowResult 	= 0;
					int iColResult	= 0;

					if(pL->size_get() == 0 || pR->size_get() == 0)
					{//return empty matrix of double
					}
					else if(pL->size_get() == 1)
					{
						iRowResult = pR->rows_get();
						iColResult = pR->cols_get();
					}
					else if (pR->size_get() == 1)
					{
						iRowResult = pL->rows_get();
						iColResult = pL->cols_get();
					}
					else if(pL->cols_get() == pR->cols_get())
					{
						iRowResult = pL->rows_get();
						iColResult = pR->rows_get();
					}

					pResult = new Double(iRowResult, iColResult, pL->isComplex() || pR->isComplex());


					int iResult = DivideDoubleByDouble(pL, pR, pResult->getAsDouble());
					if(iResult)
					{//manage errors
					}

/*
					if(pR->size_get() == 1)
					{
						double *pReal			= NULL;
						double *pImg			= NULL;

						double *pdblRealL	= pL->real_get();
						double *pdblImgL	= pL->img_get();
						double pdblRealR	= pR->real_get() == NULL ? 0 : pR->real_get()[0];
						double pdblImgR		= pR->img_get() == NULL ? 0 : pR->img_get()[0];

						pResult						= new Double(pL->rows_get(), pL->cols_get(), &pReal, &pImg);

						for(int i = 0 ; i < pL->size_get() ; i++)
						{
							pReal[i]	= (pdblRealL == NULL ? 0 : pdblRealL[i])	/ pdblRealR;
							pImg[i]		= (pdblImgL == NULL ? 0 : pdblImgL[i])		/ pdblImgR;
						}

						if(pL->isComplex() == false && pR->isComplex() == false)
						{
							pResult->getAsDouble()->complex_set(false);
						}
					}
					else if(pR->size_get() == 1)
					{//add pL with each element of pR
						double *pReal			= NULL;
						double *pImg			= NULL;

						double *pdblRealR	= pR->real_get();
						double *pdblImgR	= pR->img_get();
						double pdblRealL	= pL->real_get() == NULL ? 0 : pL->real_get()[0];
						double pdblImgL		= pL->img_get() == NULL ? 0 : pL->img_get()[0];

						pResult						= new Double(pR->rows_get(), pR->cols_get(), &pReal, &pImg);

						for(int i = 0 ; i < pR->size_get() ; i++)
						{
							pReal[i]	= pdblRealL * (pdblRealR == NULL ? 0 : pdblRealR[i])	-  pdblImgL	* (pdblImgR == NULL ? 0 : pdblImgR[i]);
							pImg[i]		= pdblRealL	* (pdblImgR == NULL ? 0 : pdblImgR[i])		+  pdblImgL	* (pdblRealR == NULL ? 0 : pdblRealR[i]);
						}

						if(pL->isComplex() == false && pR->isComplex() == false)
						{
							pResult->getAsDouble()->complex_set(false);
						}
					}
					else
					{//matrix * matrix call atlas :(
					}
*/					result_set(pResult);
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

