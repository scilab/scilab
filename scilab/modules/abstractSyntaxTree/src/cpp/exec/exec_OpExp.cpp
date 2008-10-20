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
#include <string>

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

		if(TypeR != TypeL)
		{
			Location loc = e.right_get().location_get();
			std::ostringstream os;
			os << "Incompatible types";
			os << loc.location_string_get() << std::endl;
			string szErr(os.str());
			throw szErr;
		}

		switch(e.oper_get())
		{
		case OpExp::plus :
			{
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL = (Double*)execMeL.result_get();
					Double *pR = (Double*)execMeR.result_get();
					Double *pResult = NULL;

					if(pL->size_get() == 1)
					{//add pL with each element of pR
						double *pReal	 = NULL;
						double *pImg	 = NULL;

						double *pdblRealR	= pR->real_get();
						double *pdblImgR	= pR->img_get();
						double pdblRealL	= pL->real_get() == NULL ? 0 : pL->real_get()[0];
						double pdblImgL		= pL->img_get() == NULL ? 0 : pL->img_get()[0];

						pResult				= new Double(pR->rows_get(), pR->cols_get(), &pReal, &pImg);

						for(int i = 0 ; i < pR->size_get() ; i++)
						{
							pReal[i]	= pdblRealL	+ (pdblRealR == NULL ? 0 : pdblRealR[i]);
							pImg[i]		= pdblImgL	+ (pdblImgR == NULL ? 0 : pdblImgR[i]);
						}

						if(pL->isComplex() == false && pR->isComplex() == false)
						{
							pResult->complex_set(false);
						}
					}
					else if(pR->size_get() == 1)
					{//add pL with each element of pR
						double *pReal	 = NULL;
						double *pImg	 = NULL;

						double *pdblRealL	= pL->real_get();
						double *pdblImgL	= pL->img_get();
						double pdblRealR	= pR->real_get() == NULL ? 0 : pR->real_get()[0];
						double pdblImgR		= pR->img_get() == NULL ? 0 : pR->img_get()[0];

						pResult				= new Double(pL->rows_get(), pL->cols_get(), &pReal, &pImg);

						for(int i = 0 ; i < pL->size_get() ; i++)
						{
							pReal[i]	= (pdblRealL == NULL ? 0 : pdblRealL[i])	+ pdblRealR;
							pImg[i]		= (pdblImgL == NULL ? 0 : pdblImgL[i])		+ pdblImgR;
						}

						if(pL->isComplex() == false && pR->isComplex() == false)
						{
							pResult->complex_set(false);
						}
					}
					else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())//same dimension
					{//add pL and pR element wise
						double *pReal	 = NULL;
						double *pImg	 = NULL;

						pResult				= new Double(pR->rows_get(), pR->cols_get(), &pReal, &pImg);
						double *pdblRealR	= pR->real_get();
						double *pdblImgR	= pR->img_get();
						double *pdblRealL	= pL->real_get();
						double *pdblImgL	= pL->img_get();

						for(int i = 0 ; i < pR->size_get() ; i++)
						{
							pReal[i]	= (pdblRealL == NULL ? 0 : pdblRealL[i]) + (pdblRealR == NULL ? 0 : pdblRealR[i]);
							pImg[i]		= (pdblImgL == NULL ? 0 : pdblImgL[i])	 + (pdblImgR == NULL ? 0 : pdblImgR[i]);
						}

						if(pL->isComplex() == false && pR->isComplex() == false)
						{
							pResult->complex_set(false);
						}
					}
					else
					{
						return;//inconsistent addition
					}
					result_set(pResult);
				}
				else if(TypeR == GenericType::RealBool)
				{
					//nothing, all in macro : %b_+_b
				}
				else if(TypeR == GenericType::RealString)
				{
					String *pL = (String*)execMeL.result_get();
					String *pR = (String*)execMeR.result_get();
					String *pResult = NULL;

					if(pL->size_get() == 1)
					{//concat pL with each element of pR
						pResult = new String(pR->rows_get(), pR->cols_get());
						int iCommonLen	=	strlen(pL->string_get(0,0));

						for(int i = 0 ; i < pR->rows_get() ; i++)
						{
							for(int j = 0 ; j < pR->cols_get() ; j++)
							{
								int iLen	= strlen(pR->string_get(i, j));
								char* psz = new char[iLen + iCommonLen + 1];
								memset(psz, 0x00, iLen + iCommonLen + 1);
								memcpy(psz, pL->string_get(0,0), iCommonLen * sizeof(char));
								memcpy(psz + iCommonLen, pR->string_get(i, j), iLen * sizeof(char));
								pResult->string_set(i, j, psz);
								delete psz;
							}
						}
					}
					else if(pR->size_get() == 1)
					{//concat each element of pL with pR
						pResult = new String(pL->rows_get(), pL->cols_get());
						int iCommonLen	=	strlen(pR->string_get(0,0));

						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								int iLen	= strlen(pL->string_get(i, j));
								char* psz = new char[iLen + iCommonLen + 1];
								memset(psz, 0x00, iLen + iCommonLen + 1);

								memcpy(psz, pL->string_get(i, j), iLen * sizeof(char));
								memcpy(psz + iLen, pR->string_get(0,0), iCommonLen * sizeof(char));

								pResult->string_set(i, j, psz);
								delete psz;
							}
						}
					}
					else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
					{//concat pL with pR element wise
						pResult = new String(pL->rows_get(), pL->cols_get());
						for(int i = 0 ; i < pL->rows_get() ; i++)
						{
							for(int j = 0 ; j < pL->cols_get() ; j++)
							{
								int iLenL	= strlen(pL->string_get(i, j));
								int iLenR	= strlen(pR->string_get(i, j));
								char* psz = new char[iLenL + iLenR + 1];
								memset(psz, 0x00, iLenL + iLenR + 1);

								memcpy(psz					, pL->string_get(i, j), iLenL * sizeof(char));
								memcpy(psz + iLenL	, pR->string_get(i, j), iLenR * sizeof(char));

								pResult->string_set(i, j, psz);
								delete psz;
							}
						}
					}
					else
					{
						std::ostringstream os;
						os << "inconsistent row/column dimensions";
//						os << " (" << (*row)->location_get().first_line << "," << (*row)->location_get().first_column << ")" << std::endl;
						string szErr(os.str());
						throw szErr;
					}
					result_set(pResult);
				}
				else if(TypeR == GenericType::RealUInt)
				{
				}
				else if(TypeR == GenericType::RealInt)
				{
				}

				break;
			}
		case OpExp::minus :
			{
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL = (Double*)execMeL.result_get();
					Double *pR = (Double*)execMeR.result_get();
					Double *pResult = NULL;

					if(pL->size_get() == 1)
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
							pReal[i]	= pdblRealL - (pdblRealR == NULL ? 0 : pdblRealR[i]);
							pImg[i]		= pdblImgL	- (pdblImgR == NULL ? 0 : pdblImgR[i]);
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

						double *pdblRealL	= pL->real_get();
						double *pdblImgL	= pL->img_get();
						double pdblRealR	= pR->real_get() == NULL ? 0 : pR->real_get()[0];
						double pdblImgR		= pR->img_get() == NULL ? 0 : pR->img_get()[0];

						pResult						= new Double(pL->rows_get(), pL->cols_get(), &pReal, &pImg);

						for(int i = 0 ; i < pL->size_get() ; i++)
						{
							pReal[i]	= (pdblRealL == NULL ? 0 : pdblRealL[i])	- pdblRealR;
							pImg[i]		= (pdblImgL == NULL ? 0 : pdblImgL[i])		- pdblImgR;
						}

						if(pL->isComplex() == false && pR->isComplex() == false)
						{
							pResult->complex_set(false);
						}
					}
					else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())//same dimension
					{//add pL and pR element wise
						double *pReal			= NULL;
						double *pImg			= NULL;

						pResult						= new Double(pR->rows_get(), pR->cols_get(), &pReal, &pImg);
						double *pdblRealR	= pR->real_get();
						double *pdblImgR	= pR->img_get();
						double *pdblRealL	= pL->real_get();
						double *pdblImgL	= pL->img_get();

						for(int i = 0 ; i < pR->size_get() ; i++)
						{
							pReal[i]	= (pdblRealL == NULL ? 0 : pdblRealL[i])	- (pdblRealR == NULL ? 0 : pdblRealR[i]);
							pImg[i]		= (pdblImgL == NULL ? 0 : pdblImgL[i])		- (pdblImgR == NULL ? 0 : pdblImgR[i]);
						}

						if(pL->isComplex() == false && pR->isComplex() == false)
						{
							pResult->complex_set(false);
						}
					}
					else
					{
						return;//inconsistent addition
					}
					result_set(pResult);
				}
				else if(TypeR == GenericType::RealBool)
				{
				}
				else if(TypeR == GenericType::RealString)
				{
				}
				else if(TypeR == GenericType::RealUInt)
				{
				}
				else if(TypeR == GenericType::RealInt)
				{
				}

				break;
			}
		case OpExp::times:
			{
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL			= (Double*)execMeL.result_get();
					Double *pR			= (Double*)execMeR.result_get();
					Double *pResult = NULL;

					if(pL->size_get() == 1)
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
					else if(pR->size_get() == 1)
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
							pReal[i]	= (pdblRealL == NULL ? 0 : pdblRealL[i])	* pdblRealR;
							pImg[i]		= (pdblImgL == NULL ? 0 : pdblImgL[i])		* pdblImgR;
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
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL			= (Double*)execMeL.result_get();
					Double *pR			= (Double*)execMeR.result_get();
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
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL			= (Double*)execMeL.result_get();
					Double *pR			= (Double*)execMeR.result_get();
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
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL			= (Double*)execMeL.result_get();
					Double *pR			= (Double*)execMeR.result_get();
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
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL			= (Double*)execMeL.result_get();
					Double *pR			= (Double*)execMeR.result_get();
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
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL			= (Double*)execMeL.result_get();
					Double *pR			= (Double*)execMeR.result_get();
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
				if(TypeR == GenericType::RealDouble)
				{
					Double *pL			= (Double*)execMeL.result_get();
					Double *pR			= (Double*)execMeR.result_get();
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
		default :
			break;
		}
	}
}

