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
#include "core_math.h"

#include "types_multiplication.hxx"
#include "types_addition.hxx"
#include "types_substraction.hxx"

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

		switch(e.oper_get())
		{
		case OpExp::plus :
			{
				if(TypeR == GenericType::RealDouble && TypeL == GenericType::RealDouble)
				{
					Double *pL = execMeL.result_get()->getAsDouble();
					Double *pR = execMeR.result_get()->getAsDouble();
					Double *pResult = NULL;

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
					String *pResult = NULL;

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

					MatrixPoly *pResult = AddDoubleToPoly(pR, pL);
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

					MatrixPoly *pResult = AddDoubleToPoly(pL, pR);
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
						MatrixPoly *pResult = AddPolyToPoly(pL, pR);
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
					Double *pResult = NULL;

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
					MatrixPoly *pResult = NULL;

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
					MatrixPoly *pResult = NULL;

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
					MatrixPoly *pResult = NULL;

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
					Double *pResult = NULL;

					pResult = MultiplyDoubleByDouble(pL, pR);
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
				else if(TypeL == InternalType::RealDouble && TypeR == InternalType::RealPoly)
				{
					Double *pL			    = execMeL.result_get()->getAsDouble();
					MatrixPoly *pR	    = execMeR.result_get()->getAsPoly();
					MatrixPoly *pResult = NULL;

					pResult = MultiplyDoubleByPoly(pL, pR);

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
				else if(TypeL == InternalType::RealPoly && TypeR == InternalType::RealDouble)
				{
					MatrixPoly *pL	    = execMeL.result_get()->getAsPoly();
					Double *pR			    = execMeR.result_get()->getAsDouble();
					MatrixPoly *pResult = NULL;

					pResult = MultiplyPolyByDouble(pL, pR);

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
				break;
			}
		case OpExp::divide:
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();
					Double *pResult = NULL;


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
							pResult->complex_set(false);
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
							pResult->complex_set(false);
						}
					}
					else
					{//matrix * matrix call atlas :(
					}
					result_set(pResult);
				}
				break;
			}
		case OpExp::eq :
			{
				if(TypeL == GenericType::RealDouble && TypeR == GenericType::RealDouble)
				{
					Double *pL			= execMeL.result_get()->getAsDouble();
					Double *pR			= execMeR.result_get()->getAsDouble();
					Bool *pResult = NULL;

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->bool_set(i, j, pL->real_get(i, j) == dblRef);
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
								pResult->bool_set(i, j, dblRef == pR->real_get(i, j));
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
								pResult->bool_set(i, j, pL->real_get(i, j) == pR->real_get(i, j));
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
					Bool *pResult = NULL;

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->bool_set(i, j, pL->real_get(i, j) != dblRef);
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
								pResult->bool_set(i, j, dblRef != pR->real_get(i, j));
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
								pResult->bool_set(i, j, pL->real_get(i, j) != pR->real_get(i, j));
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
					Bool *pResult = NULL;

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->bool_set(i, j, pL->real_get(i, j) < dblRef);
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
								pResult->bool_set(i, j, dblRef < pR->real_get(i, j));
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
								pResult->bool_set(i, j, pL->real_get(i, j) < pR->real_get(i, j));
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
					Bool *pResult = NULL;

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->bool_set(i, j, pL->real_get(i, j) <= dblRef);
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
								pResult->bool_set(i, j, dblRef <= pR->real_get(i, j));
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
								pResult->bool_set(i, j, pL->real_get(i, j) <= pR->real_get(i, j));
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
					Bool *pResult = NULL;

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->bool_set(i, j, pL->real_get(i, j) > dblRef);
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
								pResult->bool_set(i, j, dblRef > pR->real_get(i, j));
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
								pResult->bool_set(i, j, pL->real_get(i, j) > pR->real_get(i, j));
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
					Bool *pResult = NULL;

					if(pR->size_get() == 1)
					{
						pResult				= new Bool(pL->rows_get(), pL->cols_get());
						double dblRef	= pR->real_get(0,0);
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								pResult->bool_set(i, j, pL->real_get(i, j) >= dblRef);
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
								pResult->bool_set(i, j, dblRef >= pR->real_get(i, j));
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
								pResult->bool_set(i, j, pL->real_get(i, j) >= pR->real_get(i, j));
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
	}
}

